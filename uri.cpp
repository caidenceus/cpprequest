#include <cctype>
#include <stdio.h>
#include <iostream>

#include "cppr_str.h"
#include "uri.h" // ParserError, Uri

#define NPOS std::string::npos


static bool validUriSchemeCharacter(char const c) {
  if (cppr::isAlphaChar(c) || cppr::isDigitChar(c) || c == '+' || c == '-' || c == '.')
    return true;
  return false;
}


// URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
cppr::Uri cppr::parse_uri(std::string uri) {
  cppr::Uri rtn;

  std::string::iterator head = uri.begin();
  std::string::iterator tail = uri.end();

  if (head == tail || !cppr::isAlphaChar(*head)) {
    throw ParseError{
      "Invalid URI. Please refer to RFC 3986 Section 3: "
      "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
    };
  }

  for (; head != tail && validUriSchemeCharacter(*head); ++head)
    rtn.scheme.push_back(*head);

  std::string authority = std::string(head, tail);
  if (head == tail || authority.substr(0, 3) != "://") {
    throw ParseError{
      "Invalid URI. Please refer to RFC 3986 Section 3: "
      "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
    };
  }
  authority = authority.substr(3);

  // Fragment appears at the end of the string
  int const fragment_index = authority.find('#');
  if (fragment_index != NPOS) {
    rtn.fragment = authority.substr(fragment_index + 1);
    authority.resize(fragment_index);
  }

  // Query appears directpy before fragment
  int const query_index = authority.find('?');
  if (query_index != NPOS) {
    rtn.query = authority.substr(query_index + 1);
    authority.resize(query_index);
  }

  // At this point, URI = [authority] path. Path always begins with '/'
  int const path_index = authority.find('/');
  if (path_index != NPOS) {
    rtn.path = authority.substr(path_index);
    authority.resize(path_index);
  }
  else {
    rtn.path = "/"; // Default request path
  }

  // [username [:password ] @] host
  std::string user_pass;
  int const host_index = authority.find('@');
  if (host_index != NPOS) {
    user_pass = authority.substr(0, host_index);

    int const password_index = user_pass.find(':');
    if (password_index != NPOS) {
      rtn.user = user_pass.substr(0, password_index);
      rtn.password = user_pass.substr(password_index + 1);
    }
    else {
      rtn.user = user_pass;
    }
    rtn.host = authority.substr(host_index + 1);
  }
  else {
    rtn.host = authority;
  }
  
  // Optional port host [:port]
  int const port_index = rtn.host.find(':');
  if (port_index != NPOS) {
    rtn.port = rtn.host.substr(port_index + 1);
    rtn.host.resize(port_index);
  }

  return rtn;
}

