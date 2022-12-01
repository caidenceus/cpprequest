#include "cpprequest.h"


void cppr::Response::parse_response()
{
	// Parse the status line
	std::string status_line = this->raw.substr(0, this->raw.find("\r\n"));

	size_t http_version = status_line.find(" ");
	this->status_line.http_version = status_line.substr(0, http_version);
	status_line = status_line.substr(http_version + 1);

	size_t status_code = status_line.find(" ");
	this->status_line.status_code = atoi(status_line.substr(0, status_code).c_str());
	this->status_line.reason_phrase = status_line.substr(status_code + 1);

	// Truncate the status line
	this->raw = this->raw.substr(this->raw.find("\r\n") + 1);

	// Parse the response headers. The + 2 is because we want \r\n at the end of the string
	std::string headers = this->raw.substr(0, this->raw.find("\r\n\r\n") + 2);

	while (headers.length() > 0) {
		size_t line_break = headers.find("\r\n");
		if (line_break == std::string::npos) {
			break;
		}
		std::string current_header = headers.substr(0, line_break);
		
		size_t delim = current_header.find(":");
		std::string header_key = current_header.substr(0, delim);
		
		// delim + 2 to get substring after ": "
		std::string header_value = current_header.substr(delim + 2);

		this->headers.push_back(std::pair<std::string, std::string>{header_key, header_value});
		headers = headers.substr(line_break + 1);
	}

	// After the headers, the rest of the response is the raw body
	this->raw = this->raw.substr(this->raw.find("\r\n\r\n") + 4);
}