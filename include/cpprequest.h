#ifndef CPPREQUEST_H__
#define CPPREQUEST_H__

#include "../src/uri.h" // Uri

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

#if defined(_WIN32) || defined(__CYGWIN__)
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#include <winsock2.h>
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#else
#include <sys/socket.h>
#endif // defined(_WIN32) || defined(__CYGWIN__)

namespace cppr
{
    struct HttpVersion final
    {
        std::uint16_t major;
        std::uint16_t minor;
    };


    using Header = std::pair<std::string, std::string>;
    using Headers = std::vector<Header>;


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
        int addr_family;
        std::string host;
        std::uint16_t port;

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
        Request(
            const std::string method,
            const std::string uri,
            const std::uint16_t port = 80,
            const HttpVersion http_version = HttpVersion{ 1, 1 },
            const int addr_family = AF_UNSPEC
        );

        /**
         * @brief Send this request on the wire and fill out a Response object.
         *
         * Note: The "Host: <host IP address>" header is used automatically.
         *
         * @param response The response object to store response data in.
         * @return The response status code or -1 on error.
         */
        int send(Response &response);

        /**
         * @brief Send a request and do not wait for a response.
         *
         * Note: The "Host: <host IP address>" header is used automatically.
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
         * @brief Add a URL parameter to the query.
         *
         * Note: A request with URL parameters uses the
         *       "Content-Type: application/x-www-form-urlencoded" and
         *       "Content-Length: <size>" headers automatically.
         *
         * @param key The key of the URL parameter.
         * @param value The value of the URL parameter.
         */
        void add_url_parameter(std::string key, std::string value);

        /**
         * @breif Close socket descriptor and call WSACleanup if on Windwos.
         *
         * @return 0 on success, -1 otherwise.
         */
        int close();

        ~Request() { this->close(); }
    }; // class Request
} // namespace cppr


#endif // CPPREQUEST_H__
