#pragma once

#include <string>
#include <utility>
#include <vector>

#include "uri.h"


namespace cppr {

  struct HttpVersion final {
    uint16_t major;
    uint16_t minor;
  };


  enum class RequestVerb: std::uint8_t {
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    OPTIONS,
    TRACE,
    CONNECT,
    DELETE
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
      AfInet protocol;
      RequestVerb verb;
      Headers headers;
      std::string http_version;

      void const write_request(std::string &request_buffer);

    public:
      // TODO: http_version shoulr be HttpVersion rather than string in Request class
      Request(std::string const uri,
              RequestVerb verb,
              std::string http_version = "HTTP/1.0",
              AfInet const protocol = AfInet::IPv4) 
      : uri{cppr::parse_uri(uri)},
        verb{verb},
        http_version{http_version},
        protocol{protocol}
      { ; }

      virtual ssize_t const request() = 0;
      void add_header(std::string key, std::string value);
  }; // class Request


  class Get final: public Request {
    public:
      Get(std::string uri) : Request(uri, RequestVerb::GET) { ; }
      ssize_t const request();  
  }; // class Get


  class Post final: public Request {
    public:
      Post(std::string uri) : Request(uri, RequestVerb::POST) { ; }
      // TODO: implement this
      ssize_t const request();
  }; // class Post

  // TODO: write all Verb request classes

} // namespace CppRequest

