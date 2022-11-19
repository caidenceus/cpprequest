#pragma once

#include <string>
#include <system_error>

namespace cppr {
  
  // TODO: put this in error.h
  class ParseError final: public std::logic_error {
    public:
      using std::logic_error::logic_error;
  };


  // RFC 3986 Section 3
  struct Uri final {
    std::string scheme;
    std::string user;
    std::string password;
    std::string host;
    std::string port;
    std::string path;
    std::string query;
    std::string fragment;
  };

  
  Uri parse_uri(std::string uri, std::string port);

} // namespace Uri
