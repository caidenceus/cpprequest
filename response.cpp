#include "cpprequest.h"


void cppr::Response::parse_response()
{
	// Parse the status line
	std::string status_line = this->raw.substr(0, this->raw.find("\r\n"));

	size_t http_version = status_line.find(" ");
	this->status_line.http_version = status_line.substr(0, http_version);
	status_line = status_line.substr(http_version + 1);

	size_t status_code = status_line.find(" ");
	this->status_line.status_code = atoi(status_line.substr(0, status_code).c_str());
	this->status_line.reason_phrase = status_line.substr(status_code + 1);

	// Parse the response headers
}