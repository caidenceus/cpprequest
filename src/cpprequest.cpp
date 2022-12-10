#include "config.h"
#include "cpprequest.h"
#include "error.h"
#include "socket_wrapper.h"
#include "utility.hpp"
#include "response.h"  // void parse_response(Response &r)

#include <iostream>


// TODO: move this into the ctor
cppr::Request::Request(std::string const method, 
                       std::string const uri, 
                       int const port,
                       HttpVersion const http_version) 
  : method{ method }, 
    uri{ parse_uri(uri, std::to_string(port)) }, 
    http_version{ http_version },
    headers{ Headers{} }, 
    sockfd{ -1 }, 
    serv_addr{  }, 
    host{ this->uri.host }, 
    port{ this->uri.port }
#if defined(_WIN32) || defined(__CYGWIN__)
    , winsock_initialized{ false }
#endif // defined(_WIN32) || defined(__CYGWIN__)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    this->winsock_init();
#endif // defined(_WIN32) || defined(__CYGWIN__)

    // TODO: Pass internet protocol to Request ctor
    this->sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    memset(&(this->serv_addr), 0, sizeof(this->serv_addr));

    // char domain_ip[INET6_ADDRSTRLEN];
    // memset(&domain_ip, 0, sizeof(domain_ip));

    // TODO: Convert lookup_host to take host as std::string
    // lookup_host(this->host.c_str(), domain_ip);

    // TODO: protocol agnostic
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = Htons(static_cast<std::uint16_t>(atoi(this->port.c_str())));
    this->serv_addr.sin_addr.s_addr = Inet_addr(this->host.c_str());;

    Connect(this->sockfd, (struct sockaddr*)&(this->serv_addr), sizeof(this->serv_addr));
}


void cppr::Request::write_request_header(std::string &request_buffer)
{
    std::string http_version_str = "HTTP/";
    http_version_str += std::to_string(this->http_version.major);
    http_version_str += ".";
    http_version_str += std::to_string(this->http_version.minor);

    // Request line
    request_buffer += this->method + " ";
    request_buffer += this->uri.path + " ";
    request_buffer += http_version_str + "\r\n";

    // Headers
    for (auto it = this->headers.begin(); it != this->headers.end(); ++it)
        request_buffer += (*it).first + ": " + (*it).second + "\r\n";
    request_buffer += "Host: " + this->uri.host + "\r\n";

    // Required \r\n after the request headers
    request_buffer += "\r\n";
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

    int size = 0;
    auto remaining = request_buffer.size();

    while (remaining > 0)
    {
        size += Send(this->sockfd, request_buffer.c_str(), remaining, 0);
        remaining -= size;
    }

    int bytes_rcvd{ 0 };
    do
    {
        bytes_rcvd -= Recv(
            this->sockfd, reinterpret_cast<char*>(response_buffer.data()), 
            response_buffer.size(), 0);
    } while (bytes_rcvd > 0);
 
    response.raw = std::string{ reinterpret_cast<char*>(response_buffer.data()) };
    parse_response(response);

    return response.status_line.status_code;
}


void cppr::Request::add_header(std::string const key, std::string const value)
{
    cppr::Header new_header;
    new_header.first = key;
    new_header.second = value;
    this->headers.push_back(new_header);
}


int cppr::Request::close()
{
#if defined(_WIN32) || defined(__CYGWIN__)
    if (this->winsock_initialized)
        fWSACleanup();
#endif // defined(_WIN32) || defined(__CYGWIN__)
    if (this->sockfd != -1)
        return Close(this->sockfd);
    return 0;
}