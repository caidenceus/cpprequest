#include "error.h"
#include "includes.h"


int cpprerr::get_last_error()
{
#if defined(_WIN32) || defined(__CYGWIN__)
    return fWSAGetLastError();
#else
    return errno;
#endif // defined(_WIN32) || defined(__CYGWIN__)
}