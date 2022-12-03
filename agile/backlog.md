# Epics
- [E-3] Optimize socket functions and classes
  [ ] Add AfInet to HttpStream objects
  [ ] Make lookup_host function platform agnostic
  [ ] Make lookup_host function protocol agnostic
  [ ] Add error checking to all socket wrapper functions
  [ ] HttpStream should have a get_last_error public method that calls WSAGetLastError on windows
  [ ] HttpStream private method called connect
  [ ] HttpStream private method called send
  [ ] HttpStream private method called recv
  [ ] Replace read_n_bytes and write_n_bytes with HttpStream private methods

- [E-4] Add HTTP/0.9 and HTTP/1.0 compliance (RFC 1945)
  [ ] Check that URI passed to ctors is valid
  [ ] Add all URL percent encodings
  [ ] Add status code structure
  [ ] Remove ctors from Response structure
  [ ] Response.status_line.status_code should be of type Status
  [ ] Function to parse http version (see https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp)
  [ ] Function to parse status code (see https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp)
  [ ] Function to parse reason phrase (see https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp)
  [ ] Function to parse header fields (see https://github.com/elnormous/HTTPRequest/blob/master/include/HTTPRequest.hpp)
  [ ] Add support for cuuncked transfer coding (RFC 7230)

- [E-5] Add Request classes for all HTTP 1.1 verbs
	[ ] Add header checking per HTTP version
	[ ] HEAD request class
	[ ] PUT request class
	[ ] OPTIONS request cless
	[ ] DELETE request class
	[ ] TRACE request class
	[ ] CONNECT request class

- [E-x] Obsolete HttpStream and handle sockets directly from Request functions
- [E-x] Add support for common request and response headers
- [E-x] Add GZIP Deflate compression decoding
- [E-x] Add HTTP/1.1 compliance (RFC 2616)
