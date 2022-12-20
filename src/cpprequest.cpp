#include "cpprequest.h"     // Request, HttpVersion 

#include "config.h"         // HTTP_BUFF_SIZE
#include "error.h"          // get_last_error
#include "loaddll.h"        // LoadDLLs
#include "response.h"       // parse_response
#include "socket_wrapper.h" // Socket, Connect, Htons, Inet_addr, Send, Recv
#include "uri.h"            // Uri, parse_uri, percent_encode
#include "utility.hpp"      // to_string

#include <array>
#include <cstdint>
#include <vector>

#if defined(_WIN32) || defined(__CYGWIN__)
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#include <winsock2.h>
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#else
#include <sys/socket.h>
#endif // defined(_WIN32) || defined(__CYGWIN__)


#include <iostream>


cppr::Request::Request(
    std::string const method, 
    std::string const uri, 
    std::uint16_t const port,
    HttpVersion const http_version,
    ADDRESS_FAMILY const addr_family
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
    addrinfo hints = {};
    hints.ai_family = addr_family;
    hints.ai_socktype = SOCK_STREAM;

    const char *char_port =
        this->uri.port.empty() ? std::to_string(port).c_str() : this->uri.port.c_str();

    std::string err = "Failed to get address info of " + this->uri.host;
    if (Getaddrinfo(this->uri.host.c_str(), char_port, &hints, &info) != 0)
        throw std::system_error{ cpprerr::get_last_error(), std::system_category(), err };

    this->sockfd = Socket(addr_family, SOCK_STREAM, IPPROTO_TCP);
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
    bool host_header = false;
    for (auto it = this->headers.begin(); it != this->headers.end(); ++it)
    {
        if (!host_header && (to_lower((*it).first) == "host"))
            host_header = true;
        request_buffer += (*it).first + ": " + (*it).second + "\r\n";
    }

    if (!host_header)
        request_buffer += "Host: " + this->uri.host + "\r\n";

    // Add the Content-Type and Content-Length headers if necessary
    if (this->method == "POST")
    {
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

    if (!dlls_loaded)
    {
        std::string msg = "Unable to load DLLs";
        throw std::system_error{cpprerr::get_last_error(), std::system_category(), msg};
    }

    WSADATA wsaData;
    const auto error = fWSAStartup(MAKEWORD(2, 2), &wsaData);
    if (error != 0)
        throw std::system_error{ error, std::system_category(), "WSAStartup failed" };

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        fWSACleanup();
        throw std::runtime_error{ "Invalid WinSock version" };
    }

    this->winsock_initialized = true;
}
#endif // defined(_WIN32) || defined(__CYGWIN__)


int cppr::Request::send(cppr::Response &response)
{
    std::array<std::uint8_t, HTTP_BUFF_SIZE> response_buffer;
    std::vector<std::uint8_t> raw_response;

    std::string request_buffer;
    this->write_request_header(request_buffer);
    std::cout << request_buffer;

    int size = 0;
    auto remaining = request_buffer.size();

    while (remaining > 0)
    {
        size += Send(this->sockfd, request_buffer.c_str(), remaining, 0);
        remaining -= size;
    }

    // TODO: put this into while (1) loop to receive more than 4096 bytes
    Recv(this->sockfd, reinterpret_cast<char*>(response_buffer.data()), response_buffer.size(), 0);
 
    response.raw = std::string{ reinterpret_cast<char*>(response_buffer.data()) };
    parse_response(response);

    return response.status_line.status_code;
}


int cppr::Request::blind_send()
{
    // TODO: Consider making header a member variable and writing it only once
    //       so that blind send can send requests faster
    std::string request_buffer;
    this->write_request_header(request_buffer);

    int size = 0;
    auto remaining = request_buffer.size();

    while (remaining > 0)
    {
        size += Send(this->sockfd, request_buffer.c_str(), remaining, 0);
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
