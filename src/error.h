#ifndef CPPR_ERROR_H__
#define CPPR_ERROR_H__

#include <system_error>

namespace cpprerr
{
    class RequestError final : public std::logic_error 
    {
    public:
        using std::logic_error::logic_error;
    };


    class SocketIoError final : public std::runtime_error 
    {
    public:
        using std::runtime_error::runtime_error;
    };


    class BufferOverflowError final : public std::overflow_error 
    {
    public:
        using std::overflow_error::overflow_error;
    };


    class ParseError final : public std::logic_error 
    {
    public:
        using std::logic_error::logic_error;
    };

    class ResponseError final : public std::logic_error 
    {
    public:
        using std::logic_error::logic_error;
    };

    class HttpVersionError final : public std::logic_error 
    {
    public:
        using std::logic_error::logic_error;
    };

    /**
     * @brief Get the most recent error.
     * 
     * On Windows, return WSAGetLastError, on Linux return errno.
     */
    int get_last_error();
} // namespace cpprerr
#endif // CPPR_ERROR_H__