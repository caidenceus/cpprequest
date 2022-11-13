#include "request.h"
#include "url.h"

#include <iostream>

void cppr::Request::add_header(std::string const key, std::string const value) {
  std::string new_header = key + ": " + value;
  this->headers.push_back(new_header);
}


void const cppr::Request::write_request(std::string &request_buffer) {
  request_buffer += this->http_verb + " ";
  request_buffer += Url::parse_domain(this->url);
  request_buffer += " ";
  request_buffer += Url::parse_resource(this->url);
  request_buffer += this->http_version + "\r\n";

  for (auto it = this->headers.begin(); it != this->headers.end(); ++it) {
      request_buffer += *it + "\r\n";
  }
}


void const cppr::Get::request() {
  std::string request_buffer;
  this->write_request(request_buffer);
  std::cout << request_buffer << "\n\n";
}
