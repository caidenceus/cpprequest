#ifndef CPPR_UTILITY_HPP__
#define CPPR_UTILITY_HPP__

#include "error.h"

#include <string>

/**
 * @brief Get the lowercase representation of an uppercase character.
 *
 * @param c An uppercase character.
 * @return The lowercase representation of c.
 */
char to_lower(const char c);

/**
 * @brief Convert all uppercase characters in a string to their lowercase representation.
 *
 * @param str The string to convert to lowercase.
 * @return The lowercase representation of str.
 */
std::string to_lower(const std::string str);

/**
 * @brief Check it a character is whitespace.
 *
 * @param c The character to check.
 * @return True if c is whitespace, false otherwise.
 */
bool is_white_space(char const c);

/**
 * @brief Check it a character is a digit [0-9].
 *
 * @param c The character to check.
 * @return True if c is a digit, false otherwise.
 */
bool is_digit(char const c);

/**
 * @brief Check it a character is an alphabetical character [a-z,A-Z].
 *
 * @param c The character to check.
 * @return True if c is an alphabetical character, false otherwise.
 */
bool is_alpha(char const c);

/**
 * @brief Check it a character is an US ASCii visible character.
 *
 * @param c The character to check.
 * @return True if c is a valid HTML field character, false otherwise.
 */
bool is_visible_character(const char c);

/**
 * @brief Check it a character is a valid HTML header key character.
 *
 * @param c The character to check.
 * @return True if c is a valid HTML key character, false otherwise.
 */
bool valid_header_key_char(const char c);


/**
 * @brief Cast a digit to an uintxx_t.
 *
 * @param c The digit to cast.
 * @return The uintxx_t casted digit.
 */
template <typename T, typename C, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
T digit_to_uint(const C c)
{
    // DIGIT
    if (c >= 0x30 && c <= 0x39)
        return static_cast<T>(c - 0x30); // 0 - 9
    throw cpprerr::ResponseError{ "Invalid digit" };
}

#endif // CPPR_UTILITY_HPP__
