#include "cpprequest.h"
#include "error.h"
#include "includes.h"
#include "utility.hpp"
#include "response.h"

#include <iostream>

template<class Iterator>
static Iterator parse_status_code(Iterator begin, Iterator end, cppr::StatusLine &status_line)
{
    std::string status_code{ "" };

    auto i = begin;
    while (i != end && is_digit(*i))
        status_code.push_back(static_cast<char>(*i++));

    if (std::distance(begin, i) != 3)
        throw cpprerr::ResponseError{ "Invalid response" };

    status_line.status_code = static_cast<std::uint16_t>(atoi(status_code.c_str()));
    return i;
}


template<class Iterator>
static Iterator parse_status_reason(Iterator begin, Iterator end, cppr::StatusLine &status_line)
{
    std::string status_reason{ "" };
    auto i = begin;

    // TODO: there are more characters that can be in the reason phrase than alpha and whitespace
    while (i != end && (is_alpha(*i) || is_white_space(*i)))
        status_reason.push_back(static_cast<char>(*i++));

    status_line.reason_phrase = status_reason;
    return i;
}


template<class Iterator>
static Iterator parse_http_version(Iterator begin, Iterator end, cppr::StatusLine &status_line)
{
    auto b = begin;

    if (b == end || *b++ != 'H')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == end || *b++ != 'T')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == end || *b++ != 'T')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == end || *b++ != 'P')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    if (b == end || *b++ != '/')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };

    if (b == end)
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    const auto major_version = digit_to_uint<std::uint16_t>(*b++);

    if (b == end || *b++ != '.')
        throw cpprerr::ResponseError{ "Invalid HTTP version" };

    if (b == end)
        throw cpprerr::ResponseError{ "Invalid HTTP version" };
    const auto minor_version = digit_to_uint<std::uint16_t>(*b++);

    status_line.http_version = cppr::HttpVersion{ major_version, minor_version };
    return b;
}


template<class Iterator>
static Iterator parse_status_line(Iterator begin, Iterator end, cppr::StatusLine &status_line)
{
    auto i = parse_http_version(begin, end, status_line);
    if (i == end || *i++ != ' ')
        throw cpprerr::ResponseError{ "Invalid HTTP response" };

    i = parse_status_code(i, end, status_line);
    if (i == end || *i++ != ' ')
        throw cpprerr::ResponseError{ "Invalid HTTP response" };

    i = parse_status_reason(i, end, status_line);
    if (i == end || *i++ != '\r')
        throw cpprerr::ResponseError{ "Invalid HTTP response" };

    if (i == end || *i++ != '\n')
        throw cpprerr::ResponseError{ "Invalid HTTP response" };

    // i now points to the first character of the first header key
    return i;
}


template<class Iterator>
static Iterator parse_header(Iterator begin, Iterator end, cppr::Headers& headers)
{
    std::string header_key{ "" };
    auto i = begin;

    while (valid_header_key_char(*i))
        header_key.push_back(*i++);

    if (i == end || *i++ != ':')
        throw cpprerr::ResponseError{ "Invalid response header" };
    if (i == end || *i++ != ' ')
        throw cpprerr::ResponseError{ "Invalid response header" };

    std::string header_value{ "" };

    while (valid_header_value_char(*i))
        header_value.push_back(*i++);

    if (i == end || *i++ != '\r')
        throw cpprerr::ResponseError{ "Invalid response header" };
    if (i == end || *i++ != '\n')
        throw cpprerr::ResponseError{ "Invalid response header" };

    headers.push_back(cppr::Header{ header_key, header_value });
    return i;
}


template<class Iterator>
static Iterator parse_headers(Iterator begin, Iterator end, cppr::Headers &headers)
{
    auto i = begin;
    std::array<std::uint8_t, 2> crlf = { '\r', '\n' };
    std::cout << "Parsing some headers\n\n";

    while (1)
    {
        auto header_begin_iter = i;
        auto header_end_iter = std::search(header_begin_iter, end, crlf.cbegin(), crlf.cend());
        if (header_end_iter == end)
            return i;
        i = parse_header(i, end, headers);
    }
    return i;
}


// This function assumes the raw response is saved in response.raw
void parse_response(cppr::Response &r)
{
    std::array<std::uint8_t, 4> header_end = { '\r', '\n', '\r', '\n' };
    const auto header_end_iter = std::search(
        r.raw.cbegin(), r.raw.cend(), header_end.cbegin(), header_end.cend());

    // TODO: Make this an error
    if (header_end_iter == r.raw.cend())
    {
        std::cout << "Request failed";
        return;
    }

    auto i = parse_status_line(r.raw.cbegin(), header_end_iter, r.status_line);
    i = parse_headers(i, header_end_iter, r.headers);
}
