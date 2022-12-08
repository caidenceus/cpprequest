#include "error.h"
#include "includes.h"
#include "loaddll.h"
#include "socket_wrapper.h"


std::uint16_t Htons(std::uint16_t hostshort)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return fhtons(hostshort);
#else
	return htons(hostshort);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


std::uint32_t Inet_addr(const char* cp)
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
    auto result = fconnect(sockfd, addr, addrlen);

    // Interrupted function call
    while (result == -1 && cpprerr::get_last_error() == WSAEINTR)
        result = fconnect(sockfd, addr, addrlen);

    if (result == SOCKET_ERROR)
    {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(CONNECT ERROR) Failed to connect" };

        switch (error)
        {
        case WSAEADDRNOTAVAIL: 
            msg = "(CONNECT ERROR) Invalid address; failed to connect";
            break;
        case WSAENETUNREACH: 
            msg = "(CONNECT ERROR) Host network is unreachable";
            break;
        case WSAEHOSTUNREACH: 
            msg = "(CONNECT ERROR) Host is unreachable";
            break;
        case WSAETIMEDOUT: 
            msg = "(CONNECT ERROR) The connect function timed out";
            break;
        default:
            break;
        }

        throw std::system_error{ error, std::system_category(), msg };
    }
#else
    auto result = connect(sockfd, addr, addrlen);

    while (result == -1 && errno == EINTR)
        result = connect(sockfd, addr, addrlen);

    throw std::system_error{ errno, std::system_category(), "Failed to connect" };
#endif // defined(_WIN32) || defined(__CYGWIN__)

    return result;
}


// TODO: buf should be void instead of char
ssize_t Send(int sockfd, const char* buffer, size_t len, int flags)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    // auto result = fsend(sockfd, reinterpret_cast<const char*>(buffer), static_cast<int>(length), 0);
    auto result = fsend(sockfd, buffer, static_cast<int>(len), flags);

    while (result == -1 && cpprerr::get_last_error() == WSAEINTR)
        result = fsend(sockfd, buffer, static_cast<int>(len), flags);
#else
    auto result = send(sockfd, buffer, static_cast<int>(len), flags);

    while (result == -1 && errno == EINTR)
        result = send(sockfd, buffer, static_cast<int>(len), flags);
#endif // defined(_WIN32) || defined(__CYGWIN__)

    if (result == -1)
        throw std::system_error{ cpprerr::get_last_error(), std::system_category(), "Failed to send data" };

    return static_cast<ssize_t>(result);
}


// TODO: buf should be void instead of char
ssize_t Recv(int sockfd, char* buffer, size_t len, int flags)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    // auto result = recv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);
    auto result = frecv(sockfd, buffer, static_cast<int>(len), flags);

    while (result == -1 && cpprerr::get_last_error() == WSAEINTR)
        result = frecv(sockfd, buffer, static_cast<int>(len), flags);
#else
    auto result = recv(sockfd, buffer, static_cast<int>(len), flags);

    while (result == -1 && cpprerr::get_last_error() == EINTR)
        result = frecv(sockfd, buffer, static_cast<int>(len), flags);

#endif // defined(_WIN32) || defined(__CYGWIN__)

    if (result == -1)
        throw std::system_error{ cpprerr::get_last_error(), std::system_category(), "Failed to read data" };

    return static_cast<ssize_t>(result);
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