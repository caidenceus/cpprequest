[ ] Fix -Weffc++ errors
[ ] Fix -Wsign-conversion errors

[X] Add response structure
    [X] response text
    [X] status code
    [X] response headers
    [X] response http version
    [X] content length

[ ] Move Response struct into separate file
[ ] Move internet protocol structures into separate file

[ ] Function to parse response for status code
[ ] Function to parse response for headers
[ ] Function to parse response for content length

[ ] Make Get::request() fill out a response object
[ ] Make Post::request() fill out a response object

[ ] Figure out good buffer size for response

[ ] Investigate encodings
    [ ] gzip
    [ ] deflate

[ ] Write gzip decode algorithm

[ ] Write deflate algorithm if applicable

[ ] Write DELETE method
    [ ] Figure out http body
    [ ] Figure out required HTTP headers
