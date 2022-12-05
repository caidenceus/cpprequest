Due date: 12/17/2022

## Mission: Add early optimizations and refactoring
Notes
    - Epic 3 (Optimize socket functions and classes) is too large to do on its own, so we will add a few user stories from
      this epic into all active sprints until it is complete.
	- This sprint will be longer than usual to accomodate for studying for finals.

## [E-2] Early refactoring and optimizations
### General
	[X] Add an UNLICENSE to the root of the project
		- Add an unlicense.txt file; for more information, please refer to <http://unlicense.org/>
	[ ] Rewrite documentation for read_n_bytes and write_n_bytes
		- We are using send and recv functions now rather than read and write
		- Documentation to reflects the implementation change
	[X] Write function and class docstrings in public library header files
		- Functions and classes are documented using cygen style docstrings
	[X] Move errors out of cppr, they should be in a separate file/namespace
		- error.h and error.cpp contain all errors currently in cppr
		- Errors have been removed from cppr and resice in cppr_error
	[X] Operating system agnostic get last error function
		- On Linux returns errno and on windows returns WSAGetLastError
		- This function lives in cppr_error
    [X] Remove duplicate header files
		- Four header files live in ./include/ as well as ./ remove all non public headers from ./include/
		- Remove public facing headers from ./

### Misc
	[ ] Change Linux compiler to G++ rather than GCC
		- Library compiles after changing CC line in Makefile from gcc to g++
	[X] Add a function to close a socket connection in HttpStream
		- HttpStream::close function closes this->sockfd
	[X] Close HttpStream after making request
		- Post::request and Get::request close socet after reading from it

### String parsing
	[ ] Inside of valid_method_per_http_version, vectors of valid methods should be global
	[X] Parse HTTP version function
		- Takes a string and returns cppr::HttpVersion
		- Throw error of HttpVersion is invalid
	[X] Inside of cppr::Request::write_request_header in the for loop, make header checking case insensitive
		- For loop to cast headers to lowecase when searching for host header
		- String helper function to cast characters to lowecase
	[X] Make sure HTTP response status line is valid before parsing it in parse_response
		- Break parse_response into multiple functions, this function will be called parse_response_status_line
		- parse_response_status_line to check to make sure response is valid
		    - Helper function to parse HTTP version
		    - Helper function to parse status code
	[ ] Make sure HTTP headers are valid before parsing them in parse_response
		- Break parse_response into multiple functions, this function will be called parse_response_headers
		- parse_response_headers to check to make sure response is valid
	[ ] Make sure response.raw gets instantiated to empty string if there is no response body
	[ ] Add HttpResponseError
		- Add an error in cppr::error
		- Throw this error in response parsing functions

### Error handling
	[ ] Add error handling to HttpStream::init() for all socket/network functions
		- Reference https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp
	[ ] Add error handling to write_n_bytes
		- Reference https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp
	[ ] Add error handling to read_n_bytes
		- Reference https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp


## [E-3] Optimize socket functions and classes
    [ ] HttpStream private method called socket
		- Reference https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp
	[X] Load Winsock DLLs inside of HttpStream automatically on windows
		- Inside a new function called HttpStream::winsock_init DLLs are loaded automatically on windows
    [X] Call WSAStartup inside of HttpStream automatically on windows
		- Inside a new function called HttpStream::winsock_init DLLs are loaded automatically on windows
    [X] Call WSACleanup inside of HttpStream automatically on windows
		- This WSACleanup is called in HttpStream::close on windows
