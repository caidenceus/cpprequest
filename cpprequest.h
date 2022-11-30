#pragma once

#include "includes.h"

namespace cppr
{
#if defined(_WIN32) || defined(__CYGWIN__)
    /**
    * @brief Load DLL files needed for this library on Windows/
    * 
    * If running on Windows, this should be the first function called in main
    * to load the Winsock DLL.
    */
    BOOL LoadDLLs(void);
#endif // if defined(_WIN32) || defined(__CYGWIN__)

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

      void write_request_header(std::string &request_buffer);

    public:
      Request(std::string const uri,
              std::string const method,
              int const port = 80,
              HttpVersion const http_version = HttpVersion::OneDotOne)
      : uri{cppr::parse_uri(uri, std::to_string(port))},
        method{method},
        http_version{http_version}
      { ; }

      Request(std::string const uri,
              std::string const method,
              HttpVersion const version)
      : Request(uri, method, 80, version) { ; }

      /**
       * @brief Send this request on the wire and fill out a Response object.
       *
       * @param response The response object to store response data in.
       */
      virtual ssize_t request(cppr::Response &response) = 0;
      void add_header(std::string key, std::string value);

      virtual ~Request() = default;
  }; // class Request


  class Get final: public Request {
    public:
      Get(std::string const uri) : Request(uri, "GET") { ; }
      Get(std::string const uri, cppr::HttpVersion const version) : Request(uri, "GET", version) { ; }

      Get(std::string const uri, int const port) : Request(uri, "GET", port) { ; }
      Get(std::string const uri, int const port, cppr::HttpVersion const version) : Request(uri, "GET", port, version) { ; }

      ssize_t request(cppr::Response &response) override;

      virtual ~Get() = default;
  }; // class Get


  class Post final: public Request {
    public:
      Post(std::string const uri) : Request(uri, "POST") { ; }
      Post(std::string const uri, cppr::HttpVersion version) : Request(uri, "POST", version) { ; }

      Post(std::string const uri, int const port) : Request(uri, "POST", port) { ; }
      Post(std::string const uri, int const port, cppr::HttpVersion const version) : Request(uri, "POST", port, version) { ; }

      ssize_t request(cppr::Response &response) override;

      virtual ~Post() = default;
  }; // class Post
}
