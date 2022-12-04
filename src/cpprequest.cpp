#include "config.h"
#include "cpprequest.h"
#include "error.h"
#include "socket_io.h"
#include "utility.h"


static std::string printable_http_version(cppr::HttpVersion http_version)
{
    std::string rtn{ "HTTP / 1.1" };

    switch (http_version) {
        // If no version number is spcified, HTTP/0.9 is used
        case cppr::HttpVersion::ZeroDotNine : rtn = "";
            break;
        case cppr::HttpVersion::OneDotZero : rtn = "HTTP/1.0";
            break;
        case cppr::HttpVersion::OneDotOne : rtn = "HTTP/1.1";
            break;
        case cppr::HttpVersion::TwoDotZero : rtn = "HTTP/2.0";
            break;
        case cppr::HttpVersion::ThreeDotZero : rtn = "HTTP/3.0";
            break;
        default :
            break;
    }
    return rtn;
}


void cppr::Request::add_header(std::string const key, std::string const value)
{
    cppr::Header new_header;
    new_header.first = key;
    new_header.second = value;
    this->headers.push_back(new_header);
}


static bool valid_method_per_http_version(cppr::HttpVersion version, std::string verb)
{
    std::vector<std::string> validZeroDotNine =
        { "GET" };
    std::vector<std::string> validOneDotZero =
        { "GET", "HEAD", "POST" };
    std::vector<std::string> validOneDotOne = 
        { "GET", "HEAD", "PATCH", "POST", "PUT", "OPTIONS", "DELETE" };

    // TODO: Add HTTP 2.0 and 3.0
    switch (version) {
        case cppr::HttpVersion::ZeroDotNine:
            return (std::find(validZeroDotNine.begin(), validZeroDotNine.end(), verb) != validZeroDotNine.end());
            break;
        case cppr::HttpVersion::OneDotZero:
            return (std::find(validOneDotZero.begin(), validOneDotZero.end(), verb) != validOneDotZero.end());
            break;
        case cppr::HttpVersion::OneDotOne:
            return (std::find(validOneDotOne.begin(), validOneDotOne.end(), verb) != validOneDotOne.end());
            break;
        default :
            return false;
    }
}


void cppr::Request::write_request_header(std::string &request_buffer)
{
    std::string http_version_str = printable_http_version(this->http_version);  

    if (valid_method_per_http_version(this->http_version, this->method)) {
        request_buffer += this->method + " ";
    } else {
        std::string err{
            "WARNING: " + this->method + " is an invalid method for " +
            (http_version_str == "" ? "HTTP/0.9" : http_version_str) + ".\n"
        };
        throw cpprerr::RequestError{err};
    }

    request_buffer += this->uri.path + " ";
    request_buffer += http_version_str + "\r\n";

    // HTTP/0.9 does not support headers
    if (this->http_version < cppr::HttpVersion::OneDotZero) {
        request_buffer += "\r\n";
        return;
    }

    bool host_header = false;
    for (auto it = this->headers.begin(); it != this->headers.end(); ++it) 
    {
        if (!host_header && to_lower((*it).first) == "host")
            host_header = true;
        request_buffer += (*it).first + ": " + (*it).second + "\r\n";
    }

    // Host header required for HTTP/1.1
    if (this->http_version <= cppr::HttpVersion::OneDotOne && !host_header) {
        request_buffer += "Host: " + this->uri.host + "\r\n";
    }

    // Required \r\n after the request headers
    request_buffer += "\r\n";
}


ssize_t cppr::Get::request(cppr::Response &response)
{
    ssize_t error;
    std::string request_buffer;
    char response_buffer[HTTP_BUFF_SIZE];
    this->write_request_header(request_buffer);

    HttpStream stream{ this->uri };
    stream.init();
    error = stream.data_stream(request_buffer, response_buffer, sizeof(response_buffer));
    stream.close();
 
    if (error != 0) {
        throw cpprerr::SocketIoError{ "Socket error: unable to write response buffer.\n" };
        return -1;
    }
  
    response.raw = std::string{ response_buffer };
    response.parse_response();

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
        throw cpprerr::SocketIoError{ "Socket error: unable to write response buffer.\n" };
        return -1;
    }

    response.raw = std::string{ response_buffer };
    response.parse_response();

    return response.status_line.status_code;
}
