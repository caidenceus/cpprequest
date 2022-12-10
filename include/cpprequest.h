#pragma once

#include "../src/definition.h"
#include "../src/includes.h"

namespace cppr
{
    struct HttpVersion final
    {
        std::uint16_t major;
        std::uint16_t minor;
    };

    using Header = std::pair<std::string, std::string>;
    using Headers = std::vector<Header>;

    // RFC 3986 Section 3
    struct Uri final
    {
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


    struct StatusLine final
    {
        HttpVersion http_version;
        std::uint16_t status_code;
        std::string reason_phrase;
    };


    struct Response final
    {
        StatusLine status_line{ HttpVersion{ 1, 1 }, 0, "request not sent" };
        Headers headers{};
        std::string raw{};
    };


    class Request final
    {
    private:
        // HTTP implementation variables
        std::string method;
        Uri uri;
        HttpVersion http_version;
        Headers headers;

        // Socket implementation variables
        int sockfd;
        struct sockaddr_in serv_addr;
        std::string host;
        std::string port;

        /**
         * @brief Write the request header for this object.
         *
         * @param request_buffer The string to write the rewuest header to.
         */
        void write_request_header(std::string& request_buffer);

#if defined(_WIN32) || defined(__CYGWIN__)
        bool winsock_initialized;

        /**
         * @brief Initialize program for calling functions from Winsock.
         *
         * If on Windows, this function is called in the constructor and calls WSAStartup()
         * as well as loads functions from ws2_lib.dll.
         */
        void winsock_init();
#endif // defined(_WIN32) || defined(__CYGWIN__)

    public:
        /**
         * @brief Construct a request object.
         * 
         * @param method The request method (GET, POST, DELETE, etc.).
         * @param uri The URI string.
         * @param port The port to send the request to.
         * @param http_version THe version of HTTP to use for the request.
         */
        Request(std::string const method, 
                std::string const uri, 
                int const port = 80,
                HttpVersion const http_version = HttpVersion{ 1, 1 });

        /**
         * @brief Send this request on the wire and fill out a Response object.
         *
         * @param response The response object to store response data in.
         * @return The response status code or -1 on error.
         */
        int send(Response &response);

        /**
         * @brief Send a request and do not wait for a response. 
         * 
         * @return 0 on success or -1 on error.
         */
        int blind_send();

        /**
         * @brief Add a header to this request object.
         *
         * @param key The key of the header key-value pair.
         * @param value The value of the header key-value pair.
         */
        void add_header(std::string key, std::string value);

        /**
         * @breif Close socket descriptor and call WSACleanup if on Windwos. 
         */
        int close();

        ~Request() { this->close(); }
    }; // class Request
}
