#include <cctype>
#include <stdio.h>
#include <iostream>

#include "uri.h" // ParserError, Uri
#include "utility.h" // is_alpha, is_digit 

#define NPOS std::string::npos


static bool validUriSchemeCharacter(char const c) {
  if (cppr::is_alpha(c) || cppr::is_digit(c) || c == '+' || c == '-' || c == '.')
    return true;
  return false;
}


// URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
cppr::Uri cppr::parse_uri(std::string uri) {
  size_t index;
  cppr::Uri rtn;

  std::string::iterator head = uri.begin();
  std::string::iterator tail = uri.end();

  if (head == tail || !cppr::is_alpha(*head)) {
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
  index = authority.find('#');
  if (index != NPOS) {
    rtn.fragment = authority.substr(index + 1);
    authority.resize(index);
  }

  // Query appears directpy before fragment
  index = authority.find('?');
  if (index != NPOS) {
    rtn.query = authority.substr(index + 1);
    authority.resize(index);
  }
  
  // Percent encode the query
  rtn.query = url_encode(rtn.query);

  // At this point, URI = [authority] path. Path always begins with '/'
  index = authority.find('/');
  if (index != NPOS) {
    rtn.path = authority.substr(index);
    authority.resize(index);
  }
  else {
    rtn.path = "/"; // Default request path
  }

  // [username [:password ] @] host
  std::string user_pass;
  index = authority.find('@');
  if (index != NPOS) {
    user_pass = authority.substr(0, index);

    index = user_pass.find(':');
    if (index != NPOS) {
      rtn.user = user_pass.substr(0, index);
      rtn.password = user_pass.substr(index + 1);
    }
    else {
      rtn.user = user_pass;
    }
    rtn.host = authority.substr(index + 1);
  }
  else {
    rtn.host = authority;
  }
  
  // Optional port host [:port]
  index = rtn.host.find(':');
  if (index != NPOS) {
    rtn.port = rtn.host.substr(index + 1);
    rtn.host.resize(index);
  }
  else {
    rtn.port = "80";
  }

  return rtn;
}

