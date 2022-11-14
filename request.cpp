// TODO: remove adjunct headers
#include "request.h"
#include "socket_io.h"
#include "uri.h"
#include "socket_util.h"

#include <stdio.h>
#include <iostream>

#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


static std::string printable_http_version(cppr::HttpVersion http_version) {
  std::string rtn{ "HTTP/" };
  rtn += std::to_string(http_version.major);
  rtn += ".";
  rtn += std::to_string(http_version.minor);
  return rtn;
}


// TODO: add header checking per http version
void cppr::Request::add_header(std::string const key, std::string const value) {
  cppr::Header new_header;
  new_header.first = key;
  new_header.second = value;
  this->headers.push_back(new_header);
}


void const cppr::Request::write_request(std::string &request_buffer) {
  switch (this->verb) {
    case cppr::RequestVerb::GET : request_buffer += "GET ";
      break;
    case cppr::RequestVerb::POST : request_buffer += "POST ";
      break;
    case cppr::RequestVerb::PUT : request_buffer += "PUT ";
      break;
    case cppr::RequestVerb::HEAD : request_buffer += "HEAD ";
      break;
    case cppr::RequestVerb::PATCH : request_buffer += "PATCH ";
      break;
    case cppr::RequestVerb::OPTIONS : request_buffer += "OPTIONS ";
      break;
    case cppr::RequestVerb::TRACE : request_buffer += "TRACE ";
      break;
    case cppr::RequestVerb::CONNECT : request_buffer += "CONNECT ";
      break;
    case cppr::RequestVerb::DELETE : request_buffer += "DELETE ";
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
  if (this->http_version.major == 1 && this->http_version.minor == 1 && !host_header)
    request_buffer += "Host: " + this->uri.host + "\r\n";

  // TODO: request body

  // Required \r\n after the request body
  request_buffer += "\r\n";
}


// TODO: do we need to overload this function?
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
  stream.write(request_buffer);
  stream.read(response_buffer, sizeof(response_buffer));
  std::cout << response_buffer;  // debug
  stream.end();

  return 0;
}
