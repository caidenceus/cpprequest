#pragma once

#include "includes.h"

char to_lower(const char c);
std::string to_lower(const std::string str);
bool is_white_space(char const c);
bool is_digit(char const c);
bool is_alpha(char const c);
bool valid_header_key_char(const char c);
bool valid_header_value_char(const char c);

/**
* @brief Escape characters in URIs with their percent codes.
*
* @param query The string to escape with percent codes.
* @return The percent encoded string.
*/
std::string url_encode(const std::string query);


template <typename T, typename C, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
T digit_to_uint(const C c)
{
    // DIGIT
    if (c >= 0x30 && c <= 0x39)
        return static_cast<T>(c - 0x30); // 0 - 9
    throw cpprerr::ResponseError{ "Invalid digit" };
}