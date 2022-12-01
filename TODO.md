# =========================== Begin Sprint 1 11/28/2022 ===========================
## Create cross-platform API for sending primitive GET and POST requests

### Misc
 - [X] Remove adjunct includes
 - [ ] Directory structure should have src/ include/ and bib/ directiroes
 - [ ] Make spacing consistent. Four spaces per indentation.

### General
- [ ] Fix -Weffc++ errors (gcc)
- [ ] Fix -Wsign-conversion errors (gcc)
- [ ] Fix /W4 errors (cl)
- [ ] Fix /WX errors (cl)
- [X] Add README.md
- [X] Create an include.h that has all includes based on OS
- [X] Create definition that has all typedefs for Windows

### Response
- [X] Add response structure
- [X] Move Response struct into separate file
- [ ] Figure out good buffer size for response
- [ ] Should user specify buffer size in config.h? A: Yes, HTTP_BUFF_SIZE
- [X] Separate status line, header fields, and response body in Response object
- [ ] Add error checking to parse_response

### Socket
- [X] Move internet protocol structures into separate file
- [X] Rewrite socket read function to be faster
- [X] Rewrite socket write function to be faster
- [ ] Move close(sockfd) from HttpStream::data_stream into a separate instance method
- [ ] Make wrapper function for close(sockfd)
- [ ] Add error handling to socket wrapper functions
- [ ] Abstract OS specific errors into common cppr::error errors

### String parsing
- [X] Function to parse response HTTP version
- [X] Function to parse response for status code
- [X] Function to parse response for headers
- [X] Function to parse response for content length

### Request
- [X] Arguments to request functions to fill out Response by reference
- [X] Make Get::request() fill out a response object
- [ ] Make Post::request() fill out a response object

# =========================== End Sprint 1 12/04/2022 ===========================
# =========================== Begin Sprint 2 12/06/2022 ===========================
## Add request methods for all HTTP/1.1 versions with compliance with RFC 2616

# =========================== End Sprint 2 ===========================
# =========================== Begin Sprint 3 ===========================
## Add support for compression algorithms

# =========================== End Sprint 3 ===========================