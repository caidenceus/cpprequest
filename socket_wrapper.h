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
#include "loaddll.h"

typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
#else
#  include <arpa/inet.h>
#  include <stdint.h>
#endif  // if defined(_WIN32) || defined(__CYGWIN__)


uint16_t Htons(uint16_t hostshort);