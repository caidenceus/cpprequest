#ifndef DOMINUS_DEFINITIONS_H__
#define DOMINUS_DEFINITIONS_H__

#ifdef _WIN32
#if defined(_MSC_VER)
#include <BaseTsd.h>
#endif // defined(_MSC_VER)
typedef SSIZE_T ssize_t;
#endif // ifndef _WIN32

#endif // ifndef DOMINUS_DEFINITIONS_H__