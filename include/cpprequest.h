#pragma once

#include "../src/definition.h"
#include "../src/includes.h"

namespace cppr
{
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

    // RFC 3986 Section 3
    struct Uri final {
        std::string scheme{ };
        std::string user{ };
        std::string password{ };
        std::string host{ };
        std::string port{ };
        std::string path{ };
        std::string query{ };
        std::string fragment{ };
    };

    /**
     * @brief Parse a URI and return a Uri object.
     *
     * @param uri The URI to parse.
     * @param port The port to connect to. This is overridden if a port is specified in the URI.
     * @return The filled URI object.
     */
    Uri parse_uri(std::string uri, std::string port);


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

        /**
         * @brief Parse the HTTP response to fill out the instance variables of this object. 
         */
        void parse_response();
    };


    class Request {
    protected:
        Uri uri;
        std::string method;
        HttpVersion http_version;
        Headers headers;

        /**
         * @brief Write the request header for this object.
         * 
         * @param request_buffer The string to write the rewuest header to.
         */
        void write_request_header(std::string &request_buffer);

    public:
        /**
         * @brief Construct a request object.
         * 
         * @param uri The URI string.
         * @param method The request method (GET, POST, DELETE, etc.).
         * @param port The port to send the request to.
         * @param http_version THe version of HTTP to use for the request.
         */
        Request(std::string const uri,
            std::string const method,
            int const port = 80,
            HttpVersion const http_version = HttpVersion::OneDotOne)
        : uri{parse_uri(uri, std::to_string(port))},
          method{method},
          http_version{http_version},
	      headers{Headers{}}
        { ; }

        /**
         * @brief Construct a request using the default HTTP port 80.
         * 
         * @param uri The URI string.
         * @param method The request method (GET, POST, DELETE, etc.).
         * @param http_version THe version of HTTP to use for the request.
         */
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

        /**
         * @brief Add a header to this request object.
         *
         * @param key The key of the header key-value pair.
         * @param value The value of the header key-value pair.
         */
        void add_header(std::string key, std::string value);

        virtual ~Request() = default;
    }; // class Request


    class Get final: public Request {
    public:
        /**
         * @brief Construct a GET request object from a URI.
         *
         * @param uri The URI string.
         */
        Get(std::string const uri) : Request(uri, "GET") { ; }

        /**
         * @brief Construct a GET request object from URI and specify the HTTP version.
         *
         * @param uri The URI string.
         * @param http_version THe version of HTTP to use for the request.
         */
        Get(std::string const uri, HttpVersion const version) : Request(uri, "GET", version) { ; }

        /**
         * @brief Construct a GET request object using a URI and specify the port number.
         *
         * @param uri The URI string.
         * @param port The port to send the request to.
         */
        Get(std::string const uri, int const port) : Request(uri, "GET", port) { ; }

        /**
         * @brief Construct a GET request object and specify the port number and HTTP version.
         *
         * @param uri The URI string.
         * @param port The port to send the request to.
         * @param http_version THe version of HTTP to use for the request.
         */
        Get(std::string const uri, int const port, HttpVersion const version) : Request(uri, "GET", port, version) { ; }

        /**
         * @brief Send this request on the wire and fill out a Response object.
         *
         * @param response The response object to store response data in.
         */
        ssize_t request(cppr::Response &response) override;

        virtual ~Get() = default;
    }; // class Get


    class Post final: public Request {
    public:
        /**
         * @brief Construct a POST request object from a URI.
         *
         * @param uri The URI string.
         */
        Post(std::string const uri) : Request(uri, "POST") { ; }

        /**
         * @brief Construct a POST request object from URI and specify the HTTP version.
         *
         * @param uri The URI string.
         * @param http_version THe version of HTTP to use for the request.
         */
        Post(std::string const uri, HttpVersion version) : Request(uri, "POST", version) { ; }

        /**
         * @brief Construct a POST request object using a URI and specify the port number.
         *
         * @param uri The URI string.
         * @param port The port to send the request to.
         */
        Post(std::string const uri, int const port) : Request(uri, "POST", port) { ; }

        /**
         * @brief Construct a POST request object and specify the port number and HTTP version.
         *
         * @param uri The URI string.
         * @param port The port to send the request to.
         * @param http_version THe version of HTTP to use for the request.
         */
        Post(std::string const uri, int const port, HttpVersion const version) : Request(uri, "POST", port, version) { ; }

        /**
         * @brief Send this request on the wire and fill out a Response object.
         *
         * @param response The response object to store response data in.
         */
        ssize_t request(Response &response) override;

        virtual ~Post() = default;
    }; // class Post
}
