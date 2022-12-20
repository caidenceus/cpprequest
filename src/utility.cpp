#include "utility.hpp"

#include <string>
#include <unordered_map>


// Lowercase a character
char to_lower(const char c)
{
    char mutable_char = c;
    // A - Z
    if (c >= 0x41 && c <= 0x5A)
        return mutable_char + 0x20;
    return c;
}


// Lowercase a string
std::string to_lower(const std::string str)
{
    std::string lower{ };
    for (auto it = str.begin(); it != str.end(); ++it)
        lower += to_lower(*it);
    return lower;
}


// RFC 7230, 3.2.3. WhiteSpace
bool is_white_space(char const c)
{
    return c == 0x20 || c == 0x09; // space or tab
}


// RFC 5234, Appendix B.1. Core Rules
bool is_digit(char const c)
{
    return c >= 0x30 && c <= 0x39; // 0 - 9
}


// RFC 5234, Appendix B.1. Core Rules
bool is_alpha(char const c)
{
    return
        (c >= 0x61 && c <= 0x7A) || // a - z
        (c >= 0x41 && c <= 0x5A); // A - Z
}


// Printable US ASCii characters (incluides is_alpha, is_digit, and is_white_space)
bool is_visible_character(const char c)
{
    return (c >= 0x20 && c <= 0x7F);
}


// RFC 7230, 3.2.6. Field Value Components
bool valid_header_key_char(const char c)
{
    return 
        c == 0x21 || // !
        c == 0x23 || // #
        c == 0x24 || // $
        c == 0x25 || // %
        c == 0x26 || // &
        c == 0x27 || // '
        c == 0x2A || // *
        c == 0x2B || // +
        c == 0x2D || // -
        c == 0x2E || // .
        c == 0x5E || // ^
        c == 0x5F || // _
        c == 0x60 || // `
        c == 0x7C || // |
        c == 0x7E || // ~
        is_alpha(c) ||
        is_digit(c);
};
