#pragma once

#include "includes.h"


bool is_white_space(char const c);
bool is_digit(char const c);
bool is_alpha(char const c);

/**
* @brief Escape characters in URIs with their percent codes.
*
* @param query The string to escape with percent codes.
* @return The percent encoded string.
*/
std::string url_encode(const std::string query);
