#include "config.h"
#include "cpprequest.h"
#include "error.h"
#include "socket_io.h"
#include "utility.hpp"
#include "response.h"  // void parse_response(Response &r)

#include <iostream>


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
    ssize_t error;
    std::array<std::uint8_t, HTTP_BUFF_SIZE> response_buffer;
    std::vector<std::uint8_t> raw_response;

    std::string request_buffer;
    this->write_request_header(request_buffer);

    HttpStream stream{ this->uri };
    stream.init();
    error = stream.data_stream(
        request_buffer, reinterpret_cast<char*>(response_buffer.data()), response_buffer.size());
    stream.close();
 
    if (error != 0) {
        throw cpprerr::SocketIoError{ "Socket error: unable to write response buffer." };
        return -1;
    }
 
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
