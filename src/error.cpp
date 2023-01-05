#include "error.h"

#if defined(_WIN32) || defined(__CYGWIN__)
#include "loaddll.h"    // fWSAGetLastError
#endif // defined(_WIN32) || defined(__CYGWIN__)

#include <system_error>

int cpprerr::get_last_error()
{
#if defined(_WIN32) || defined(__CYGWIN__)
    return fWSAGetLastError();
#else
    return errno;
#endif // defined(_WIN32) || defined(__CYGWIN__)
}
