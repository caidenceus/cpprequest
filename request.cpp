#include "request.h"
#include "socket_io.h"
#include "uri.h"
#include "socket_util.h"

#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <string.h>


static std::string printable_http_version(cppr::HttpVersion http_version) {
  std::string rtn{ "HTTP/" };

  switch (http_version) {
    case cppr::HttpVersion::ZeroDotNine : return rtn + "0.9";
      break;
    case cppr::HttpVersion::OneDotZero : return rtn + "1.0";
      break;
    case cppr::HttpVersion::OneDotOne : return rtn + "1.1";
      break;
    case cppr::HttpVersion::TwoDotZero : return rtn + "2.0";
      break;
    case cppr::HttpVersion::ThreeDotZero : return rtn + "3.0";
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


bool valid_verb_per_http_version(cppr::HttpVersion version, std::string verb) {
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


void const cppr::Request::write_request(std::string &request_buffer) {
  switch (this->verb) {
    case cppr::RequestVerb::GET:
      if (valid_verb_per_http_version(this->http_version, "GET"))
        request_buffer += "GET ";
      // throw error
      break;
    case cppr::RequestVerb::POST:
      if (valid_verb_per_http_version(this->http_version, "POST"))
        request_buffer += "POST ";
      // throw error
      break;
    case cppr::RequestVerb::PUT:
      if (valid_verb_per_http_version(this->http_version, "PUT"))
        request_buffer += "PUT ";
      // throw error
      break;
    case cppr::RequestVerb::HEAD:
      if (valid_verb_per_http_version(this->http_version, "HEAD"))
        request_buffer += "HEAD ";
      // throw error
      break;
    case cppr::RequestVerb::PATCH:
      if (valid_verb_per_http_version(this->http_version, "PATCH"))
        request_buffer += "PATCH ";
      // throw error
      break;
    case cppr::RequestVerb::OPTIONS:
      if (valid_verb_per_http_version(this->http_version, "OPTIONS"))
        request_buffer += "OPTIONS ";
      // throw error
      break;
    case cppr::RequestVerb::TRACE:
      if (valid_verb_per_http_version(this->http_version, "TRACE"))
        request_buffer += "TRACE ";
      // throw error
      break;
    case cppr::RequestVerb::CONNECT:
      if (valid_verb_per_http_version(this->http_version, "CONNECT"))
        request_buffer += "CONNECT ";
      // throw error
      break;
    case cppr::RequestVerb::DELETE:
      if (valid_verb_per_http_version(this->http_version, "DELETE"))
        request_buffer += "DELETE ";
      // throw error
      break;
    default : // Throw error TODO: write error
      break;
  }

  bool host_header = false;

  request_buffer += this->uri.path + " ";
  request_buffer += printable_http_version(this->http_version) + "\r\n";

  for (auto it = this->headers.begin(); it != this->headers.end(); ++it) {
      // TODO: headers are case insensitive, cast each header to lowercase for host checking
      if (!host_header && ( (*it).first == "Host" || (*it).first == "host" ))
        host_header = true;
      request_buffer += (*it).first + ": " + (*it).second + "\r\n";
  }

  // Host header required for HTTP/1.1
  if (this->http_version == cppr::HttpVersion::OneDotOne && !host_header)
    request_buffer += "Host: " + this->uri.host + "\r\n";

  // TODO: request body

  // Required \r\n after the request body
  request_buffer += "\r\n";
}


// TODO: return response code if applicable, -1 otherwise
// TODO: add error checking
ssize_t const cppr::Get::request() {
  std::string request_buffer;
  this->write_request(request_buffer);
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
