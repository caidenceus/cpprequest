#include "cpprequest.h"
#include "error.h"
#include "utility.hpp"


// Assumes first three digits of string are the status code
static uint16_t parse_status_code(std::string http_line)
{
	int digits = 0;
	uint16_t result = 0;

	for (auto it = http_line.begin(); digits < 3; ++it, ++digits) {
		if (it == http_line.end() || ! is_digit(*it))
			throw cpprerr::ResponseError{ "Invalid HTTP response" };
		// Multiply the digit by 10^i where i is the ones place for the digit (3 - (digits + 1))
		result += static_cast<uint16_t>(std::pow(10, 3 - (digits + 1))) * digit_to_uint<uint16_t>(*it);
	}

	return result;
}


/**
 * @brief Parse a string for a valid HTTP version.
 *
 * Note: It is assumed that the string begins with "HTTP/".
 *
 * @param http_line The string to parse for an HTTP version.
 * @return The HTTP version associated with the HTTP version string.
 */
static cppr::HttpVersion parse_http_version(std::string http_line)
{
    auto b = http_line.begin();
    auto e = http_line.end();

    if (b == e || *b++ != 'H')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == e || *b++ != 'T')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == e || *b++ != 'T')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == e || *b++ != 'P')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == e || *b++ != '/')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };

    if (b == e)
        throw cpprerr::ResponseError{ "Invalid HTTP version" };

    const auto major_version = digit_to_uint<std::uint16_t>(*b++);

    if (b == e || *b++ != '.')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };

    if (b == e)
        throw cpprerr::ResponseError{ "Invalid HTTP version" };

    const auto minor_version = digit_to_uint<std::uint16_t>(*b++);

    // TODO: This is messy
    switch (major_version)
    {
    case 0:
        if (minor_version == 9)
            return cppr::HttpVersion::ZeroDotNine;
        throw cpprerr::HttpVersionError{ "Invalid HTTP version" };
    case 1:
        switch (minor_version)
        {
        case 0:
            return cppr::HttpVersion::OneDotZero;
        case 1:
            return cppr::HttpVersion::OneDotOne;
        default:
            throw cpprerr::HttpVersionError{ "Invalid HTTP version" };
        }
    case 2:
        if (minor_version == 0)
            return cppr::HttpVersion::TwoDotZero;
        throw cpprerr::HttpVersionError{ "Invalid HTTP version" };
    case 3:
        if (minor_version == 0)
            return cppr::HttpVersion::ThreeDotZero;
        throw cpprerr::HttpVersionError{ "Invalid HTTP version" };
    default:
        throw cpprerr::HttpVersionError{ "Invalid HTTP version" };
    }
}


void cppr::Response::parse_response()
{
	// Parse the status line
	std::string raw_status_line = this->raw.substr(0, this->raw.find("\r\n"));

	size_t http_version = raw_status_line.find(" ");
	this->status_line.http_version = parse_http_version(raw_status_line.substr(0, http_version));
	raw_status_line = raw_status_line.substr(http_version + 1);

	size_t status_code = raw_status_line.find(" ");
	this->status_line.status_code = parse_status_code(raw_status_line.substr(0, status_code));
	this->status_line.reason_phrase = raw_status_line.substr(status_code + 1);

	// Truncate the status line
	this->raw = this->raw.substr(this->raw.find("\r\n") + 1);

	// Parse the response headers. The + 2 is because we want \r\n at the end of the string
	std::string header_block = this->raw.substr(0, this->raw.find("\r\n\r\n") + 2);

	while (header_block.length() > 0) {
		size_t line_break = header_block.find("\r\n");
		if (line_break == std::string::npos) {
			break;
		}
		std::string current_header = header_block.substr(0, line_break);
		
		size_t delim = current_header.find(":");
        if (delim == std::string::npos)
            throw cpprerr::ParseError{ "Invalid HTTP header received" };
		std::string header_key = current_header.substr(0, delim);
		
		// delim + 2 to get substring after ": "
		std::string header_value = current_header.substr(delim + 2);

		this->headers.push_back(std::pair<std::string, std::string>{header_key, header_value});
		header_block = header_block.substr(line_break + 1);
	}

	// After the headers, the rest of the response is the raw body
	this->raw = this->raw.substr(this->raw.find("\r\n\r\n") + 4);
}