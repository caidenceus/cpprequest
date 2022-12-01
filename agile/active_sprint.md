Due date 12/03/2022

## Mission: Create cross-platform API for sending primitive GET and POST requests

### Misc
- [X] Remove adjunct includes
    - All .cpp and .h files only include necessary header files
- [x] Directory structure should have src/ include/ and bin/ directiroes
    - All source files are in src/
    - Public facing API files are in include/
- [X] Make spacing consistent
    - All files use four spaces per indentation
- [X] Update makefiles to accomodate for new directory structure
    - Make files create working static libraries on Ubuntu and Windows
- [X] Add PLACEHOLDER.txt in bin/ so that bin/ is added to github project

### General
- [X] Fix -Weffc++ errors (gcc)
    - No Weffc++ errors
- [X] Fix -Wsign-conversion errors (gcc)
    - No -Wsign-conversion errors
- [X] Fix /W4 errors (cl)
    - No W4 errors
- [X] Add README.md
- [X] Create an include.h that has all includes based on OS
    - Include appropriate files based on platform
- [X] Create definition that has all typedefs for Windows
    - Definitions for Windows types used in socket functions

### Response
- [X] Add response structure
    - Raw response body member variable that contains the response body without HTTP headers/status line
    - StatusLine object member variable for holding the http status line
    - Member variable for holding the response headers
- [X] Figure out good buffer size for response
    - Use a macro definition for the HTTP buffer size

### Socket
- [X] Move internet protocol structures into separate file
    - Internet protocol structure moved to socket_io.h
- [X] Make wrapper function for close(sockfd)
    - Wrapper function to close sockets is platform agnostic

### String parsing
- [X] Function to parse response HTTP version
    - Fill out the HTTP version in a Response object
- [X] Function to parse response for status code
    - Fill out the status code of a Response object
- [X] Function to parse response for headers
    - Fill out the headers member variable of a response object

### Request
- [X] Arguments to request functions to fill out Response by reference
    - Classes that overload cppr::Request.request(Response &response) fill out the response object after request is made
- [X] Make Get::request() fill out a response object
- [X] Make Post::request() fill out a response object


