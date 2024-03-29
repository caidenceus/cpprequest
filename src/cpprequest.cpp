#include "cpprequest.h"     // Request, HttpVersion

#include "config.h"         // HTTP_BUFF_SIZE
#include "error.h"          // get_last_error
#include "response.h"       // parse_response
#include "socket_wrapper.h" // Socket, Connect, Htons, Inet_addr, Send, Recv
#include "uri.h"            // Uri, parse_uri, percent_encode
#include "utility.hpp"      // to_string

#include <array>
#include <cstdint>
#include <string>
#include <vector>


#if defined(_WIN32) || defined(__CYGWIN__)
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#include <Windows.h>
#include <Winsock2.h>
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#include "loaddll.h"
#else
#include <cstring>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#endif // defined(_WIN32) || defined(__CYGWIN__)


#include <iostream>


cppr::Request::Request(
    const std::string method,
    const std::string uri,
    const std::uint16_t port,
    const HttpVersion http_version,
    const int addr_family
) :
    method{ method },
    uri{ parse_uri(uri, std::to_string(port)) },
    http_version{ http_version },
    headers{ Headers{} },
    sockfd{ -1 },
    addr_family{ addr_family },
    host{ this->uri.host },
    port{ port }
#if defined(_WIN32) || defined(__CYGWIN__)
    , winsock_initialized{ false }
#endif // defined(_WIN32) || defined(__CYGWIN__)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    this->winsock_init();
#endif // defined(_WIN32) || defined(__CYGWIN__)

    addrinfo* info;
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = addr_family; // AF_UNSPEC is the default
    hints.ai_socktype = SOCK_STREAM;

    const char *char_port =
        this->uri.port.empty() ? std::to_string(port).c_str() : this->uri.port.c_str();

    if (Getaddrinfo(this->uri.host.c_str(), char_port, &hints, &info) != 0) {
        throw std::system_error{
            cpprerr::get_last_error(), std::system_category(), "Getaddrinfo failed" };
    }

    this->sockfd = Socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    Connect(this->sockfd, info->ai_addr, info->ai_addrlen);
    Freeaddrinfo(info);
}


void cppr::Request::write_request_header(std::string &request_buffer)
{
    std::string http_version_str = "HTTP/";
    http_version_str += std::to_string(this->http_version.major);
    http_version_str += ".";
    http_version_str += std::to_string(this->http_version.minor);

    // RFC 7230 Section 3.1.1. Request line
    request_buffer += this->method + " ";
    request_buffer += this->uri.path + " ";
    request_buffer += http_version_str + "\r\n";

    // Headers
    bool host_header{ false };
    bool user_agent{ false };
    for (auto it = this->headers.begin(); it != this->headers.end(); ++it) {
        if (!host_header && (to_lower((*it).first) == "host")) {
            host_header = true;
        }
        else if (!user_agent && (to_lower((*it).first) == "user-agent")) {
            user_agent = true;
        }

        request_buffer += (*it).first + ": " + (*it).second + "\r\n";
    }

    if (!host_header)
        request_buffer += "Host: " + this->uri.host + "\r\n";

    if (!user_agent)
        request_buffer += "User-Agent: " + std::string{ DEFAULT_USER_AGENT } + "\r\n";

    // Add the Content-Type and Content-Length headers if necessary
    if (this->method == "POST") {
        request_buffer += "Content-Type: application/x-www-form-urlencoded\r\n";
        request_buffer += "Content-Length: " + std::to_string(this->uri.query.length()) + "\r\n";
    }

    // Required \r\n after the request headers
    request_buffer += "\r\n";

    if (this->method == "POST")
        request_buffer += this->uri.query + "\r\n";
}


#if defined(_WIN32) || defined(__CYGWIN__)
void cppr::Request::winsock_init()
{
    BOOL dlls_loaded = LoadDLLs();

    if (!dlls_loaded) {
        std::string msg = "Unable to load DLLs";
        throw std::system_error{cpprerr::get_last_error(), std::system_category(), msg};
    }

    WSADATA wsaData;
    const auto error = fWSAStartup(MAKEWORD(2, 2), &wsaData);
    if (error != 0)
        throw std::system_error{ error, std::system_category(), "WSAStartup failed" };

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        fWSACleanup();
        throw std::runtime_error{ "Invalid WinSock version" };
    }

    this->winsock_initialized = true;
}
#endif // defined(_WIN32) || defined(__CYGWIN__)


int cppr::Request::send(cppr::Response &response)
{
    // Write to socket
    std::string request_buffer{};
    this->write_request_header(request_buffer);
    // std::cout << request_buffer;  // Debug
    size_t size{ 0 };
    size_t remaining{ request_buffer.size() };

    // TODO: Improper logic
    while (remaining > 0) {
        // Casting here is ok since error handling is done inside of Send
        size += static_cast<size_t>(Send(this->sockfd, request_buffer.c_str(), remaining, 0));
        remaining -= size;
    }

    // Read from socket
    std::array<std::uint8_t, HTTP_BUFF_SIZE> response_buffer;
    std::vector<std::uint8_t> raw_response{};
    size = 0;
    int retries{ 3 };
    int selection{};

    fd_set rset;
    struct timeval tv;

    while (retries > 0) {
        // Some Linux versions modify the timeval structure in select,
        // so for portability we reinitialize it with every select call
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 0.1 second timeout

        // Add sockfd to the set
        FD_ZERO(&rset); // Zero all fd bits in the set
        FD_SET(static_cast<unsigned int>(this->sockfd), &rset);

        // TODO write wrapper function for select
        selection = select(this->sockfd + 1, &rset, NULL, NULL, &tv);
        if (selection == -1) {
            // TODO throw error
            std::cout << "Error polling from socket";
            exit(-1);
        }

        // Select timeout
        else if (selection == 0) {
            retries--;
            continue;
        }

        // Socket is readable
        if (FD_ISSET(this->sockfd, &rset)) {
            // Casting here is ok because error handling is done inside of Recv
            size = static_cast<size_t>(
                Recv(
                    this->sockfd,
                    reinterpret_cast<char*>(response_buffer.data()),
                    response_buffer.size(),
                    0
                )
            );

            // Other end closed connection gracefully
            if (size == 0)
                break;
            raw_response.insert(
                raw_response.end(), response_buffer.begin(), response_buffer.begin() + size);
        }
        // No data waiting to be read
        else {
            retries--;
        }
    }

    response.raw = std::string{ reinterpret_cast<char*>(raw_response.data()) };
    parse_response(response);

    return response.status_line.status_code;
}


int cppr::Request::blind_send()
{
    // TODO: Consider making header a member variable and writing it only once
    //       so that blind send can send requests faster
    std::string request_buffer;
    this->write_request_header(request_buffer);

    size_t size = 0;
    size_t remaining = request_buffer.size();

    while (remaining > 0) {
        // Casting here is ok because error handling is done inside of Send
        size += static_cast<size_t>(Send(this->sockfd, request_buffer.c_str(), remaining, 0));
        remaining -= size;
    }

    return 0;
}


void cppr::Request::add_header(std::string const key, std::string const value)
{
    cppr::Header new_header;
    new_header.first = key;
    new_header.second = value;
    this->headers.push_back(new_header);
}

void cppr::Request::add_url_parameter(std::string const key, std::string const value)
{
    // TODO: urlencode this data
    this->uri.query += percent_encode(key) + "=" + percent_encode(value);
}


int cppr::Request::close()
{
    int rtn{ -1 };
    if (this->sockfd != -1)
        rtn = Close(this->sockfd);
#if defined(_WIN32) || defined(__CYGWIN__)
    if (this->winsock_initialized)
        fWSACleanup();
#endif // defined(_WIN32) || defined(__CYGWIN__)
    return rtn;
}
