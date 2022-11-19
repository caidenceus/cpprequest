#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <utility>


namespace cppr {

  using Header = std::pair<std::string, std::string>;
  using Headers = std::vector<Header>;

  class Response {
    private:
      size_t content_length = 0;

    public:
      std::string version;
      int status = -1;
      cppr::Headers headers;
      std::string raw;

      Response() = default;
      Response(const Response &) = default;
      Response(Response &&) = default;
      Response &operator=(Response &&) = default;
      ~Response() = default;
  };


  void parse_response_status_code(cppr::Response &response);


  void parse_response_http_version(cppr::Response &response);


  void parse_response_headers(cppr::Response &response);



} // namespace cppr
