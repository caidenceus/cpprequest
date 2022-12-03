Due date: 12/17/2022

## Mission: Add early optimizations and refactoring
Notes: 
    - Epic 3 (Optimize socket functions and classes) is too large to do on its own, so we will add a few user stories from
      this epic into all active sprints until it is complete.
	- This sprint will be longer than usual to accomodate for studying for finals.

- [E-2] Early refactoring and optimizations
## General
	[ ] Add an UNLICENSE to the root of the project
	[ ] Rewrite documentation for read_n_bytes and write_n_bytes
	[ ] Write function and class docstrings in public library header files

## Misc
	[ ] Change Linux compiler to G++ rather than GCC
	[ ] Add a function to close a socket connection in HttpStream
	[ ] Close HttpStream after making request

## String parsing
	[ ] Inside of valid_method_per_http_version, vectors of valid methods should be global
	[ ] Inside of cppr::Request::write_request_header in the for loop, make header checking case insensitive
	[ ] Make sure HTTP response is valid before parsing it in parse_response
	[ ] Make sure response.raw gets instantiated to empty string if there is no response body

	[ ] Add HttpResponseError
	[ ] Make sure response has headers before trying to parse them in parse_response

## Error handling
	[ ] Add error handling to HttpStream::init() for all socket/network functions
	[ ] Add error handling to write_n_bytes
	[ ] Add error handling to read_n_bytes


- [E-3] Optimize socket functions and classes
    [ ] HttpStream private method called socket
	[ ] Load Winsock DLLs inside of HttpStream automatically on windows
    [ ] Call WSAStartup inside of HttpStream automatically on windows
    [ ] Call WSACleanup inside of HttpStream automatically on windows
