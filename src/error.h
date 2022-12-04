#include "includes.h"


namespace cpprerr
{
    class RequestError final : public std::logic_error {
    public:
        using std::logic_error::logic_error;
    };


    class SocketIoError final : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };


    class BufferOverflowError final : public std::overflow_error {
    public:
        using std::overflow_error::overflow_error;
    };


    class ParseError final : public std::logic_error {
    public:
        using std::logic_error::logic_error;
    };
}