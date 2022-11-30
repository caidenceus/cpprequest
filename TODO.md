# =========================== Begin Sprint 1 ===========================
## Create cross-platform API for sending GET and POST requests

### Misc
 - [X] Remove adjunct includes
 - [ ] Directory structure should have src/ include/ and bib/ directiroes

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
- [ ] Figure out good buffer size for response or investigate using std::string
- [ ] Should user specify buffer size in config.h?
- [ ] Make compliant with RFC 2616 section 5

### Socket
- [X] Move internet protocol structures into separate file
- [X] Rewrite socket read function to be faster
- [X] Rewrite socket write function to be faster
- [ ] Move close(sockfd) from HttpStream::data_stream into a separate instance method
- [ ] Make wrapper function for close(sockfd)
- [ ] Add error handling to socket wrapper functions
- [ ] Abstract OS specific errors into common cppr::error errors

### String parsing
- [ ] Function to parse response HTTP version
- [ ] Function to parse response for status code
- [ ] Function to parse response for headers
- [ ] Function to parse response for content length

### Request
- [X] Arguments to request functions to fill out Response by reference
- [ ] Make Get::request() fill out a response object
- [ ] Make Post::request() fill out a response object
- [ ] Make compliant with RFC 2616 section 6

# =========================== End Sprint 1 ===========================
# =========================== Begin Sprint 2 ===========================
## Add request methods for all HTTP/1.1 versions

# =========================== End Sprint 2 ===========================
# =========================== Begin Sprint 3 ===========================
## Add support for compression algorithms

# =========================== End Sprint 3 ===========================