# cpprequest

cpprequest is a simple cross-platform static library for sending and receiving HTTP data.

## User guides

User guides can be found at ./agile/deliverables/sprint_x_date/user_guide.html

## Example 1 (GET request on Linux)
A basic example of sending a GET request with the request line "GET /public/index.php HTTP/1.1" to 127.0.0.1 on port 80.

```cpp
#include <iostream>
#include "cpprequest.h"

int main() {
    cppr::Response response_buff;
    cppr::Get req{ "http://127.0.0.1/public/index.php" };
    req.request(response_buff);

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
    cppr::Get req{ "http://127.0.0.1/public/index.php", 8080 };

    req.add_header("Cache-Control": "no-cache");
    req.add_header("Connection", "Keep-Alive");
    req.add_header("Keep-Alive", "timeout=5, max=1000");
  
    req.request(response_buff);
  
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
    cppr::Post req{ "http://127.0.0.1/public/index.php?param1=value1&param2=value 2", 8080 };
    req.add_header("Cache-Control": "no-cache");
    req.request(response_buff);
  
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
    BOOL ws2_lib_loaded = cppr::LoadDLLs();
    
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup function failed with error: %d\n", iResult);
        return -1;
    }
  
    if (ws2_lib_loaded) {
        cppr::Response response_buff;
        cppr::Get req{ "http://127.0.0.1/public/index.php" };
        req.request(response_buff);

        // Print the raw response
        std::cout << response_buff.raw;
    }

    return 0;
}
```

## Usage
### Linux
1) Execute `make` to compile.
2) Add `#include "cpprequest.h"` at the top of your file.
3) When compiling your program, add the arguments `-std=c++17 -I/path/to/cpprequest/ -L/path/to/cpprequest/ -lcpprequest` (replacing "/path/to/cpprequest/" with the full or relative path to the parent directory of this file)

### Windows
1) Run Makefile.bat to compile.
2) #include "cpprequest.h" at the top of your file
3) Link and initialize to Winsock (see example 4)
4) Create Makefile.bat and run it to compile your program and link with cpprequest.lib. An example compile script is shown below.
```bat
REM Makefile.bat
REM
REM This Makefile assumes your project has the following structure:
REM
REM    /your_project/
REM        your_code.cpp  <- cpprequest.h is included in this file
REM        Makefile.bat   <- This is the file you are currently viewing
REM        /bin/
REM        /lib/
REM            /cpprequest/  <- this is this repository

REM ========== Copy everything below this line into Makefile.bat ==========

@echo off
pushd bin

cl^
  -Zi^
  -EHsc^
  -I "../lib/cpprequest/include/"^
  -c^
  ../your_code.cpp &&^
link your_code.obj^
  cpprequest.lib^
  /LIBPATH:"../lib/cpprequest/"

popd
```

## Supported C++ standards
- C++ 17

## Supported Operating Systems
- Ubuntu Linux
- Windows 10

## Supported compilers
- GCC
- CL
