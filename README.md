# cpprequest

cpprequest is a simple cross-platform static library for sending and receiving HTTP data. Works on Windows with CL and Linux with G++.

## Example 1 (GET request on Linux)
A basic example of sending a GET request with the request line "GET /public/index.php HTTP/1.1" to 127.0.0.1 on port 80.

```cpp
#include <iostream>
#include "cpprequest.h"

int main() {
    cppr::Response response_buff;
    cppr::Request req{ "GET", "http://127.0.0.1/public/index.php" };
    req.send(response_buff);

    // Print the raw response
    std::cout << response_buff.raw;

    return 0;
}
```

## Example 2 (GET request on Linux)
Get request with user defined headers and port number.

```cpp
#include <iostream>
#include "cpprequest.h"

int main() {
    cppr::Response response_buff;
  
    // Send this request to port 8080
    cppr::Request req{ "GET", "http://127.0.0.1/public/index.php", 8080 };

    req.add_header("Cache-Control": "no-cache");
    req.add_header("Connection", "Keep-Alive");
    req.add_header("Keep-Alive", "timeout=5, max=1000");
  
    req.send(response_buff);
  
    // Print the raw response
    std::cout << response_buff.raw;

    return 0;  
}
```

## Example 3 (POST request on Linux)
Post request with simple URI query sent to port 8080. Notice the whitespace near the end of the URI. This is OK becase cpprequest will percent encode the URI query for us.

```cpp
#include <iostream>
#include "cpprequest.h"

int main() {
    cppr::Response response_buff;
  
    // Send this request to port 8080
    cppr::Request req{ "POST",  "http://127.0.0.1/public/index.php?param1=value1&param2=value 2", 8080 };
    req.add_header("Cache-Control": "no-cache");
    req.send(response_buff);
  
    // Print the raw response
    std::cout << response_buff.raw;

    return 0;  
}
```

## Example 4 (GET request on Windows 10)
```cpp
#include <iostream>
#include "cpprequest.h"

// Link to Winsock
#pragma comment(lib, "Ws2_32.lib")

int main() {
    cppr::Response response_buff;
    cppr::Request req{ "GET", "http://127.0.0.1/public/index.php" };
    req.send(response_buff);

    // Print the raw response
    std::cout << response_buff.raw;

    return 0;
}
```

# Usage
## Linux (G++)
### Notes
- Install dependencies before compiling and linking.

##### Ubuntu
- `sudo apt-add-repository universe`
- `sudo apt update`
- `sudo apt install -y g++ make`

### Compile and link
1) Compile cpprequest
    - `make`
2) Include cpprequest as a header in your program (your_program.cpp)
    - `#include "cpprequest.h"`
3) Compile your program
    - `g++ your_program.cpp -std=c++17 -I/path/to/cpprequest/include/ -L/path/to/cpprequest/ -lcpprequest -o your_program`

## Windows (CL)
### Notes
- These steps are run from a Visual Studio developer's command prompt

### Compile and link
1) Compile cpprequest
    - `./Makefile.bat`
2) Include cpprequest as a header in your program (your_program.cpp)
    - `#include "cpprequest.h"`
3) Link to Winsock
    - Add `#pragma comment(lib, "Ws2_32.lib")` above your "main" function in your_program.cpp.
4) Compile your program (your_program.cpp)
    - `cl -Zi -EHsc -std:c++17 -I "/path/to/cpprequest/include/" -c your_program.cpp`
5) Link to cpprequest 
    - `link your_program.obj cpprequest.lib /LIBPATH:"/path/to/cpprequest/"`

# Supported compilers
- G++
- CL
