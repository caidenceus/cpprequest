#pragma once

#include "definition.h"
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

    /* Begin common definitions */
    enum class HttpVersion : std::uint8_t {
        NoHttp,
        ZeroDotNine,
        OneDotZero,
        OneDotOne,
        TwoDotZero,
        ThreeDotZero
    };

    using Header = std::pair<std::string, std::string>;
    using Headers = std::vector<Header>;
    /* End common definitions */

    /* Begin URI definitions */
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
    /* End URI definitions */

    /* Begin response definitions */
    // TODO: make http_version HttpVersion rather than std::string
    struct StatusLine final {
        std::string http_version;
        ssize_t status_code;
        std::string reason_phrase;
    };

    struct Response final {
        StatusLine status_line{ "error", -1, "error"};
        Headers headers{};
        size_t content_length{ 0 };
        std::string raw{};

        Response() = default;
        Response(const Response&) = default;
        Response(Response&&) = default;
        Response& operator=(Response&&) = default;
        ~Response() = default;

        void parse_response();
    };
    /* End response definitions */


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
        : uri{parse_uri(uri, std::to_string(port))},
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
        virtual ssize_t request(Response &response) = 0;
        void add_header(std::string key, std::string value);

        virtual ~Request() = default;
    }; // class Request


    class Get final: public Request {
    public:
        Get(std::string const uri) : Request(uri, "GET") { ; }
        Get(std::string const uri, HttpVersion const version) : Request(uri, "GET", version) { ; }

        Get(std::string const uri, int const port) : Request(uri, "GET", port) { ; }
        Get(std::string const uri, int const port, HttpVersion const version) : Request(uri, "GET", port, version) { ; }

        ssize_t request(cppr::Response &response) override;

        virtual ~Get() = default;
    }; // class Get


    class Post final: public Request {
    public:
        Post(std::string const uri) : Request(uri, "POST") { ; }
        Post(std::string const uri, HttpVersion version) : Request(uri, "POST", version) { ; }

        Post(std::string const uri, int const port) : Request(uri, "POST", port) { ; }
        Post(std::string const uri, int const port, HttpVersion const version) : Request(uri, "POST", port, version) { ; }

        ssize_t request(Response &response) override;

        virtual ~Post() = default;
    }; // class Post


    namespace error {

        class RequestError final : public std::logic_error {
        public:
            using std::logic_error::logic_error;
        };


        class SocketIoError final : public std::runtime_error {
        public:
            using std::runtime_error::runtime_error;
        };


        class BufferOverflowError final : public std::overflow_error {
        public:
            using std::overflow_error::overflow_error;
        };


        class ParseError final : public std::logic_error {
        public:
            using std::logic_error::logic_error;
        };
    }
}
