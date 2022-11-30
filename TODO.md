# Misc
 - [ ] Remove adjunct includes
 - [ ] Directory structure should have src/ include/ and bib/ directiroes

# General
- [ ] Fix -Weffc++ errors (gcc)
- [ ] Fix -Wsign-conversion errors (gcc)
- [ ] Fix /W4 errors (cl)
- [ ] Fix /WX errors (cl)
- [X] Add README.md
- [ ] Create an include.h that has all includes based on OS
- [ ] Create definition that has all typedefs for Windows

# Response
- [X] Add response structure
- [X] Move Response struct into separate file
- [ ] Figure out good buffer size for response or investigate using std::string
- [ ] Should user specify buffer size in config.h?
- [ ] Investigate GZIP Deflate header and using zlib

# Socket
- [ ] Move internet protocol structures into separate file
- [ ] Rewrite socket read function to be faster
- [ ] Rewrite socket write function to be faster
- [ ] Move close(sockfd) from HttpStream::data_stream into a separate instance method
- [ ] Add error handling to socket wrapper functions
- [ ] Abstract OS specific errors into common cppr::error errors

# String parsing
- [ ] Function to parse response HTTP version
- [ ] Function to parse response for status code
- [ ] Function to parse response for headers
- [ ] Function to parse response for content length

# Request
- [X] Arguments to request functions to fill out Response by reference
- [ ] Make Get::request() fill out a response object
- [ ] Make Post::request() fill out a response object
- [ ] Write DELETE method
- [ ] Figure out http body for delete (none)
- [ ] Figure out required HTTP headers for delete

