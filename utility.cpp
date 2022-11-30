#include "utility.h"


// RFC 7230, 3.2.3. WhiteSpace
bool is_white_space(char const c) {
    return c == 0x20 || c == 0x09; // space or tab
}


// RFC 5234, Appendix B.1. Core Rules
bool is_digit(char const c) {
    return c >= 0x30 && c <= 0x39; // 0 - 9
}


// RFC 5234, Appendix B.1. Core Rules
bool is_alpha(char const c) {
    return
        (c >= 0x61 && c <= 0x7A) || // a - z
        (c >= 0x41 && c <= 0x5A); // A - Z
}


// TODO: list all valid url characters
static bool valid_url_query_char(char const c) {
    return
        (is_alpha(c) || is_digit(c)) ||
        (c == '+' || c == '.' || c == '-' || c == '_') ||
        (c == '~' || c == '=');
}

std::string url_encode(std::string const query) {
    // TODO: move to outer scope?
    std::unordered_map<char, std::string> pe;

    // TODO: add all percent encodings
    pe[' '] = "%20";
    pe['!'] = "%21";
    pe['"'] = "%22";
    pe['#'] = "%23";
    pe['$'] = "%24";
    pe['%'] = "%25";
    pe['&'] = "%26";
    pe['\''] = "%27";
    pe['('] = "%28";
    pe[')'] = "%29";
    pe['*'] = "%2A";
    pe[','] = "%2C";
    pe['/'] = "%2F";

    std::string percent_encoded;

    for (auto it = query.begin(); it != query.end(); ++it) {
        if (!valid_url_query_char(*it)) {
            percent_encoded += pe[*it];
        } else {
            percent_encoded.push_back(*it);
        }
    }

    return percent_encoded;
}
