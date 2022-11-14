#include "cppr_str.h"


// RFC 7230, 3.2.3. WhiteSpace
bool cppr::is_white_space(char const c) {
  return c == 0x20 || c == 0x09; // space or tab
};


// RFC 5234, Appendix B.1. Core Rules
bool cppr::is_digit(char const c) {
  return c >= 0x30 && c <= 0x39; // 0 - 9
}


// RFC 5234, Appendix B.1. Core Rules
bool cppr::is_alpha(char const c) {
  return
    (c >= 0x61 && c <= 0x7A) || // a - z
    (c >= 0x41 && c <= 0x5A); // A - Z
}
