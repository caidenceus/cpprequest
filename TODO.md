# General
- [ ] Fix -Weffc++ errors
- [ ] Fix -Wsign-conversion errors
- [X] Add README.md

# Response
- [X] Add response structure
- [X] Move Response struct into separate file
- [ ] Figure out good buffer size for response or investigate using std::string

# Socket
- [ ] Move internet protocol structures into separate file
- [ ] Rewrite socket read function to be faster
- [ ] Rewrite socket write function to be faster
- [ ] Move close(sockfd) from HttpStream::data_stream into a separate instance method

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

# Implementation
- [ ] Investigate gzip encoding
- [ ] Investigate deflate encoding keyword
- [ ] Write gzip decode algorithm
- [ ] Write gzip encode algorithm
- [ ] Write deflate algorithm if applicable
