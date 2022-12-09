#include "config.h"
#include "cpprequest.h"
#include "error.h"
#include "socket_io.h"
#include "utility.hpp"
#include "response.h"  // void parse_response(Response &r)

#include <iostream>


void cppr::Request::init()
{
#if defined(_WIN32) || defined(__CYGWIN__)
    this->winsock_init();
#endif // defined(_WIN32) || defined(__CYGWIN__)

    // TODO: Pass internet protocol to HttpStream ctor
    this->sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    memset(&(this->serv_addr), 0, sizeof(this->serv_addr));

    // char domain_ip[INET6_ADDRSTRLEN];
    // memset(&domain_ip, 0, sizeof(domain_ip));

    // TODO: Convert lookup_host to take host as std::string
    // lookup_host(this->host.c_str(), domain_ip);

    // TODO: protocol agnostic
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = Htons(static_cast<uint16_t>(atoi(this->port.c_str())));
    this->serv_addr.sin_addr.s_addr = Inet_addr(this->host.c_str());;

    Connect(this->sockfd, (struct sockaddr*)&(this->serv_addr), sizeof(this->serv_addr));
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


#if defined(_WIN32) || defined(__CYGWIN__)
void cppr::Request::winsock_init()
{
    BOOL dlls_loaded = LoadDLLs();

    if (!dlls_loaded)
    {
        throw std::system_error{
            cpprerr::get_last_error(), std::system_category(), "Unable to load DLLs"
        };
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


static std::string printable_http_version(cppr::HttpVersion version)
{
    std::string rtn{ "HTTP/" };

    rtn += std::to_string(version.major);
    rtn += ".";
    rtn += std::to_string(version.minor);
    return rtn;
}


void cppr::Request::add_header(std::string const key, std::string const value)
{
    cppr::Header new_header;
    new_header.first = key;
    new_header.second = value;
    this->headers.push_back(new_header);
}


// TODO Valid method for HTTP request


void cppr::Request::write_request_header(std::string &request_buffer)
{
    request_buffer += this->method + " ";
    request_buffer += this->uri.path + " ";
    request_buffer += printable_http_version(this->http_version) + "\r\n";

    bool host_header = false;
    for (auto it = this->headers.begin(); it != this->headers.end(); ++it) 
    {
        if (!host_header && to_lower((*it).first) == "host")
            host_header = true;
        request_buffer += (*it).first + ": " + (*it).second + "\r\n";
    }

    request_buffer += "Host: " + this->uri.host + "\r\n";

    // Required \r\n after the request headers
    request_buffer += "\r\n";
}


ssize_t cppr::Get::request(cppr::Response &response)
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


ssize_t cppr::Post::request(cppr::Response &response)
{
    int error;
    std::string content_length = std::to_string(this->uri.query.length());
    std::string request_buffer;
    char response_buffer[HTTP_BUFF_SIZE];

    this->add_header("Content-Type", "application/x-www-form-urlencoded");
    this->add_header("Content-Length", content_length);
    this->write_request_header(request_buffer);

    //TODO: write the body
    request_buffer += this->uri.query + "\r\n";
    HttpStream stream{ this->uri };
  
    stream.init();
    error = stream.data_stream(request_buffer, response_buffer, sizeof(response_buffer));
    stream.close();

    if (error != 0) {
        throw cpprerr::SocketIoError{ "Socket error: unable to write response buffer." };
        return -1;
    }

    response.raw = std::string{ response_buffer };
    parse_response(response);

    return response.status_line.status_code;
}
