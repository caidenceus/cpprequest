# cpprequest

cpprequest is a simple static library for sending and receiving HTTP data.

# Example
A basic example of seinding a get request

```
#include <iostream>
#include "cpprequest.h"

int main() {
  // Create a response object to hold response data
  cppr::Request request;

  // Create a Get request object instantiated from an URI
  cppr::Get get_request{ "http://127.0.0.1/public/index.php" }

  // Send the request to the server
  get_request.request();

  return 0;  
}

```
