#include "request.h"
#include "socket_io.h"
#include "uri.h"
#include "socket_util.h"
#include "error.h"

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <string.h>


static std::string printable_http_version(cppr::HttpVersion http_version) {
  std::string rtn;

  switch (http_version) {
    // If no version number is spcified, HTTP/0.9 is used
    case cppr::HttpVersion::ZeroDotNine : return rtn = "";
      break;
    case cppr::HttpVersion::OneDotZero : return rtn = "HTTP/1.0";
      break;
    case cppr::HttpVersion::OneDotOne : return rtn = "HTTP/1.1";
      break;
    case cppr::HttpVersion::TwoDotZero : return rtn = "HTTP/2.0";
      break;
    case cppr::HttpVersion::ThreeDotZero : return rtn = "HTTP/3.0";
      break;
    default:
      return rtn + "1.1";
  }
}


// TODO: add header checking per http version
void cppr::Request::add_header(std::string const key, std::string const value) {
  cppr::Header new_header;
  new_header.first = key;
  new_header.second = value;
  this->headers.push_back(new_header);
}


static bool valid_method_per_http_version(cppr::HttpVersion version, std::string verb) {
  // TODO: move these to global scope
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
    default:
      return false;
  }
}


void cppr::Request::write_request_header(std::string &request_buffer) {
  std::string http_version = printable_http_version(this->http_version);  

  if (valid_method_per_http_version(this->http_version, this->method)) {
    request_buffer += this->method + " ";
  }
  else {
    std::string err{
        "WARNING: " + this->method + " is an invalid method for " +
        (http_version == "" ? "HTTP/0.9" : http_version) + ".\n"
    };
    throw cppr::error::RequestError{err};
  }

  request_buffer += this->uri.path + " ";
  request_buffer += http_version + "\r\n";
  
  // HTTP/0.9 does not support headers
  if (this->http_version < cppr::HttpVersion::OneDotZero) {
    request_buffer += "\r\n";
    return;
  }

  bool host_header = false;

  for (auto it = this->headers.begin(); it != this->headers.end(); ++it) {
      // TODO: headers are case insensitive, cast each header to lowercase for host checking
      if (!host_header && ( (*it).first == "Host" || (*it).first == "host" ))
        host_header = true;
      request_buffer += (*it).first + ": " + (*it).second + "\r\n";
  }

  // Host header required for HTTP/1.1
  if (this->http_version == cppr::HttpVersion::OneDotOne && !host_header)
    request_buffer += "Host: " + this->uri.host + "\r\n";

  // Required \r\n after the request headers
  request_buffer += "\r\n";
}


// TODO: return response code if applicable, -1 otherwise
// TODO: add error checking
ssize_t cppr::Get::request() {
  std::string request_buffer;
  this->write_request_header(request_buffer);
  std::cout << request_buffer;  // debug

  // TODO: change this to a std::string?
  char response_buffer[65535];
  bzero(&response_buffer, sizeof(response_buffer));

  HttpStream stream{ this->uri };
  stream.init();
  stream.data_stream(request_buffer, response_buffer, sizeof(response_buffer));
  std::cout << response_buffer;
  std::cout << "\n\n";
  return 0;
}


// TODO: return response code if applicable, -1 otherwise
// TODO: add error checking
ssize_t  cppr::Post::request() {
  this->add_header("Content-Type", "application/x-www-form-urlencoded");
  std::string content_length = std::to_string(this->uri.query.length());
  this->add_header("Content-Length", content_length);

  std::string request_buffer;
  this->write_request_header(request_buffer);

  //TODO: write the body
  request_buffer += this->uri.query + "\r\n";

  std::cout << request_buffer + "\n\n";

  // TODO: change this to a std::string?
  char response_buffer[65535];
  bzero(&response_buffer, sizeof(response_buffer));

  HttpStream stream{ this->uri };
  stream.init();
  stream.data_stream(request_buffer, response_buffer, sizeof(response_buffer));
  std::cout << response_buffer;
  std::cout << "\n\n";
  return 0;
}
