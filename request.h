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
    POST
  };


  enum class AfInet: std::uint8_t {
    IPv4,
    IPv6
  };


  using Header = std::pair<std::string, std::string>;
  using Headers = std::vector<Header>;


  class Request {
    private:
      Uri uri;
      AfInet protocol;
      RequestVerb verb;
      Headers headers;
      std::string http_version;

    protected:
      void const write_request(std::string &request_buffer);

    public:
      Request(std::string const uri,
              RequestVerb verb,
              std::string http_version = "HTTP/1.1",
              AfInet const protocol = AfInet::IPv4) 
      : uri{cppr::parse_uri(uri)},
        verb{verb},
        http_version{http_version},
        protocol{protocol}
      { ; }

      virtual void const request() = 0;
      void add_header(std::string key, std::string value);
  }; // class Request


  class Get: public Request {
    public:
      Get(std::string uri) : Request(uri, RequestVerb::GET) { ; }
      void const request();  
  }; // class Get

} // namespace CppRequest

