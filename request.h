#pragma once

#include <string>
#include <vector>

#define GET "GET"
#define POST "POST"

namespace CppRequest 
{
  struct HttpVersion final {
    uint16_t major;
    uint16_t minor;
  };


  // RFC 3986 Section 3
  struct Uri final {
    std::string scheme;
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
      InternetProtocol protocol;
      RequestVerb verb;

    public:
      Request(std::string const uri,
              AfInet const protocol = AfInet::IPv4,
              RequestVerb const verb = GET) 
      : uri{parse_uri(begin(uri), end(uri))}, 
        protocol{protocol},
        verb{verb} 
      { ; }

      virtual void const request() = 0;
      void add_header(std::string key, std::string value);
  }; // class Request


  class Get: public Request {
    public:
      Get(std::string uri) : Request(url, GET) { ; }
      void const request();  
  }; // class Get

} // namespace CppRequest

