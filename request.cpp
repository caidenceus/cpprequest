#include "request.h"
#include "uri.h"

#include <iostream>


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
    default : // Throw error
      break;
  }

  request_buffer += this->uri.path + " ";
  request_buffer += this->http_version + "\r\n";

  for (auto it = this->headers.begin(); it != this->headers.end(); ++it) {
      request_buffer += (*it).first + ": " + (*it).second + "\r\n";
  }
}


void const cppr::Get::request() {
  std::string request_buffer;
  this->write_request(request_buffer);
  std::cout << request_buffer << "\n\n";
}
