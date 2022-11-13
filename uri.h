#pragma once

#include <string>

namespace Uri {
  
  class ParseError final: public std::logic_error {
    public:
      using std::logic_error::logic_error;
  };

  // RFC 3986 Section 3
  struct Uri final {
    std::string scheme;
  };
  
  static bool validUriSchemeCharacter(char c) {
    if (isAlphaChar(c) || isDigitChar(c) || c == '+' || c == '-' || c == '.')
      return true;
    return false;
  }

  Uri parse_uri(std::string uri) {
    Uri rtn;

    std::string::iterator head = uri.begin();
    std::string::iterator tail = uri.end();

    if (head == tail || !isAlphaChar(*head)) {
       throw ParseError{
         "Invalid URI. Please refer to RFC 3986 Section 3: "
         "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
       };
    }

    for (; head != tail && validUriSchemeCharacter(*head); ++head)
      rtn.scheme.push_back(*head);

    if (head == tail || head != ':' || head != '/') {
      throw ParseError{
        "Invalid URI. Please refer to RFC 3986 Section 3: "
        "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
      };
    }

    std::string authority = std::string(i, end);

    const auto fragmentPosition = authority.find('#');
    if (fragmentPosition != std::string::npos) {
      result.fragment = authority.substr(fragmentPosition + 1);
      authority.resize(fragmentPosition);
    }

    const auto queryPosition = authority.find('?');
    if (queryPosition != std::string::npos) {
      result.query = authority.substr(queryPosition + 1);
      authority.resize(queryPosition);
    }

    const auto pathPosition = authority.find('/');
    if (pathPosition != std::string::npos) {
      result.path = authority.substr(pathPosition);
      authority.resize(pathPosition);
    }
    else {
      result.path = "/";
    }

    std::string userinfo;
    const auto hostPosition = authority.find('@');
    if (hostPosition != std::string::npos) {
      userinfo = authority.substr(0, hostPosition);

      const auto passwordPosition = userinfo.find(':');
      if (passwordPosition != std::string::npos) {
        result.user = userinfo.substr(0, passwordPosition);
        result.password = userinfo.substr(passwordPosition + 1);
      }
      else {
        result.user = userinfo;
      }
      result.host = authority.substr(hostPosition + 1);
    }
    else {
      result.host = authority;
    }

    const auto portPosition = result.host.find(':');
    if (portPosition != std::string::npos) {
      result.port = result.host.substr(portPosition + 1);
      result.host.resize(portPosition);
    }
    
    return rtn;
  }
}
