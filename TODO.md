# General
[ ] Fix -Weffc++ errors
[ ] Fix -Wsign-conversion errors
[X] Add README.md

# Response
[X] Add response structure
    [X] response text
    [X] status code
    [X] response headers
    [X] response http version
    [X] content length

[X] Move Response struct into separate file
[ ] Figure out good buffer size for response

# Socket
[ ] Move internet protocol structures into separate file

# String parsing
[ ] Function to parse response HTTP version
[ ] Function to parse response for status code
[ ] Function to parse response for headers
[ ] Function to parse response for content length

# Request
[X] Arguments to request functions to fill out Response by reference
[ ] Make Get::request() fill out a response object
[ ] Make Post::request() fill out a response object

[ ] Write DELETE method
    [ ] Figure out http body
    [ ] Figure out required HTTP headers

# Implementation
[ ] Investigate encodings
    [ ] gzip
    [ ] deflate
[ ] Write gzip decode algorithm
[ ] Write gzip encode algorithm
[ ] Write deflate algorithm if applicable

