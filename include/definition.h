#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#include <BaseTsd.h>
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef uint32_t in_addr_t;
typedef SSIZE_T ssize_t;
#endif // if defined(_WIN32) || defined(__CYGWIN__)
