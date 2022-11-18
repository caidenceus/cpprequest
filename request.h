#pragma once

#include <string>
#include <utility>
#include <vector>

#include "uri.h"

#define HTTP1dot1 HttpVersion


namespace cppr {

  enum class HttpVersion: std::uint8_t {
    ZeroDotNine,
    OneDotZero,
    OneDotOne,
    TwoDotZero,
    ThreeDotZero
  };

  
  // TODO: move socket interface to another file
  enum class AfInet: std::uint8_t {
    IPv4,
    IPv6
  };


  using Header = std::pair<std::string, std::string>;
  using Headers = std::vector<Header>;

  class Request {
    protected:
      Uri uri;
      std::string method;
      HttpVersion http_version;

      Headers headers;

      void const write_request_header(std::string &request_buffer);

    public:
      Request(std::string const uri,
              std::string const method,
              HttpVersion const http_version = HttpVersion::OneDotOne) 
      : uri{cppr::parse_uri(uri)},
        method{method},
        http_version{http_version}
      { ; }

      virtual ssize_t const request() = 0;
      void add_header(std::string key, std::string value);
  }; // class Request


  class Get final: public Request {
    public:
      Get(std::string uri) : Request(uri, "GET") { ; }
      Get(std::string uri, cppr::HttpVersion version) : Request(uri, "GET", version) { ; }

      // TODO: Ctor to pass (uri, port)
      // TODO: Ctor to pass (domain, resource)
      // TODO: Ctor to pass (domain, resource, port)
      //
      // TODO: add all above ctors with http version as an argument

      ssize_t const request();

  }; // class Get


  class Post final: public Request {
    public:
      Post(std::string uri) : Request(uri, "POST") { ; }
      Post(std::string uri, cppr::HttpVersion version) : Request(uri, "POST", version) { ; }
      
      // TODO: Ctor to pass (uri, port)
      // TODO: Ctor to pass (domain, resource)
      // TODO: Ctor to pass (domain, resource, port)
      
      // TODO: add all above ctors with HTTP version as an argument
      
      // TODO: implement this
      ssize_t const request();
  }; // class Post

  // TODO: write all Verb request classes

} // namespace CppRequest

