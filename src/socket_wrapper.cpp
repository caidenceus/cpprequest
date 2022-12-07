#include "error.h"
#include "includes.h"
#include "loaddll.h"
#include "socket_wrapper.h"


uint16_t Htons(uint16_t hostshort)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return fhtons(hostshort);
#else
	return htons(hostshort);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


in_addr_t Inet_addr(const char* cp)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return finet_addr(cp);
#else
	return inet_addr(cp);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    auto result = connect(sockfd, addr, addrlen);
    while (result == -1 && cpprerr::get_last_error() == WSAEINTR)
        result = connect(sockfd, addr, addrlen);

    if (result == -1)
    {
        if (cpprerr::get_last_error() == WSAEWOULDBLOCK)
        {
            char socketErrorPointer[sizeof(int)];
            socklen_t optionLength = sizeof(socketErrorPointer);
            if (fgetsockopt(sockfd, SOL_SOCKET, SO_ERROR, socketErrorPointer, &optionLength) == -1)
                throw std::system_error{ cpprerr::get_last_error(), std::system_category(), "Failed to get socket option" };

            int socketError;
            std::memcpy(&socketError, socketErrorPointer, sizeof(socketErrorPointer));

            if (socketError != 0)
                throw std::system_error{ socketError, std::system_category(), "Failed to connect" };
        }
        else
            throw std::system_error{ cpprerr::get_last_error(), std::system_category(), "Failed to connect" };
    }
#else
    auto result = connect(sockfd, addr, addrlen);
    while (result == -1 && errno == EINTR)
        result = connect(sockfd, addr, addrlen);

    if (result == -1)
    {
        if (errno == EINPROGRESS)
        {
            int socketError;
            socklen_t optionLength = sizeof(socketError);
            if (getsockopt(endpoint, SOL_SOCKET, SO_ERROR, &socketError, &optionLength) == -1)
                throw std::system_error{ errno, std::system_category(), "Failed to get socket option" };

            if (socketError != 0)
                throw std::system_error{ socketError, std::system_category(), "Failed to connect" };
        }
        else
            throw std::system_error{ errno, std::system_category(), "Failed to connect" };
    }
#endif // defined(_WIN32) || defined(__CYGWIN__)

    return result;
}


// TODO: buf should be void instead of char
ssize_t Send(int sockfd, const char* buf, size_t len, int flags)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return fsend(sockfd, buf, len, flags);
#else
	return send(sockfd, buf, len, flags);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


// TODO: buf should be void instead of char
ssize_t Recv(int sockfd, char* buf, size_t len, int flags)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return frecv(sockfd, buf, len, flags);
#else
	return recv(sockfd, buf, len, flags);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


int Socket(int domain, int type, int protocol)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    return fsocket(domain, type, protocol);
#else
    return socket(domain, type, protocol);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


int Close(int fd)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return fclosesocket(fd);
#else
	return close(fd);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}