#pragma once


namespace cppr {

 class Response {
   private:
     size_t content_length = 0;

    public:
      std::string version;
      int status = -1;
      Headers headers;
      std::string body;

      Response() = default;
      Response(const Response &) = default;
      Response(Response &&) = default;
      Response &operator=(Response &&) = default;
      ~Response() = default;
  };


} // namespace cppr
