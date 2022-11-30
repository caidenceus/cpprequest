#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)

// TODO: remove adjunct headers
// TODO: Create a universal "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>
#include <winsock2.h>
#include <windows.h>
#include <wininet.h>
#include <windns.h>
#include <iphlpapi.h>
#include <lm.h>
#include <lmat.h>
#include <io.h>
#include <fcntl.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <assert.h>
#include <sqlext.h>
#include <vfw.h>
#include <shlobj.h>
#include <ntsecapi.h>
#include <ws2tcpip.h> // socklen_t
#include "loaddll.h"

typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef uint32_t in_addr_t;
#else
#  include <arpa/inet.h>
#  include <stdint.h>
#endif  // if defined(_WIN32) || defined(__CYGWIN__)


uint16_t Htons(uint16_t hostshort);

in_addr_t Inet_addr(const char* cp);

int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);