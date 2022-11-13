#include <cctype>
#include <stdio.h>
#include <iostream>
#include "uri.h" // ParserError, Uri


// RFC 7230, 3.2.3. WhiteSpace
        template <typename C>
        constexpr bool isWhiteSpaceChar(const C c) noexcept
        {
            return c == 32 || c == 9; // space or tab
        };

        // RFC 5234, Appendix B.1. Core Rules
        template <typename C>
        constexpr bool isDigitChar(const C c) noexcept
        {
            return c >= 48 && c <= 57; // 0 - 9
        }

        // RFC 5234, Appendix B.1. Core Rules
        template <typename C>
        constexpr bool isAlphaChar(const C c) noexcept
        {
            return
                (c >= 97 && c <= 122) || // a - z
                (c >= 65 && c <= 90); // A - Z
        }


static bool validUriSchemeCharacter(char c) {
  if (isAlphaChar(c) || isDigitChar(c) || c == '+' || c == '-' || c == '.')
    return true;
  return false;
}


Uri::Uri Uri::parse_uri(std::string uri) {
  Uri rtn;
  std::cout << uri;

  std::string::iterator head = uri.begin();
  std::string::iterator tail = uri.end();
  printf("\n\n%c\n\n%02x\n\n", *tail, *tail);

  if (head == tail || !isAlphaChar(*head)) {
    throw ParseError{
      "Invalid URI. Please refer to RFC 3986 Section 3: "
      "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
    };
  }

  std::cout << "After error 1";
  for (; head != tail && validUriSchemeCharacter(*head); ++head)
    rtn.scheme.push_back(*head);

  if (head == tail || *head != ':' || *head != '/') {
    throw ParseError{
      "Invalid URI. Please refer to RFC 3986 Section 3: "
      "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
    };
  }

  std::string authority = std::string(head, tail);

  const auto fragmentPosition = authority.find('#');
  if (fragmentPosition != std::string::npos) {
    rtn.fragment = authority.substr(fragmentPosition + 1);
    authority.resize(fragmentPosition);
  }

  const auto queryPosition = authority.find('?');
  if (queryPosition != std::string::npos) {
    rtn.query = authority.substr(queryPosition + 1);
    authority.resize(queryPosition);
  }

  const auto pathPosition = authority.find('/');
  if (pathPosition != std::string::npos) {
    rtn.path = authority.substr(pathPosition);
    authority.resize(pathPosition);
  }
  else {
    rtn.path = "/";
  }

  std::string userinfo;
  const auto hostPosition = authority.find('@');
  if (hostPosition != std::string::npos) {
    userinfo = authority.substr(0, hostPosition);

    const auto passwordPosition = userinfo.find(':');
    if (passwordPosition != std::string::npos) {
      rtn.user = userinfo.substr(0, passwordPosition);
      rtn.password = userinfo.substr(passwordPosition + 1);
    }
    else {
      rtn.user = userinfo;
    }
    rtn.host = authority.substr(hostPosition + 1);
  }
  else {
    rtn.host = authority;
  }

  const auto portPosition = rtn.host.find(':');
  if (portPosition != std::string::npos) {
    rtn.port = rtn.host.substr(portPosition + 1);
    rtn.host.resize(portPosition);
  }

  return rtn;
}

