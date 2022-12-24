#include "socket_wrapper.h"

#include "error.h"    // get_last_error
#include "loaddll.h"  // fhtons, finet_addr, fsocket, fconnect, fsend, frecf, fclosesocket

#include <cstdint>

#if defined(_WIN32) || defined(__CYGWIN__)
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#include <winsock2.h>
#include <ws2tcpip.h> // socklen_t
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#else
#include <sys/socket.h>
#endif // defined(_WIN32) || defined(__CYGWIN__)


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
     std::uint32_t net_byte_order_addr = finet_addr(cp);

    if (net_byte_order_addr == INADDR_NONE) {
        std::string msg = "(INET_ADDR) invalid IPv4 address";
        throw std::system_error{ cpprerr::get_last_error(), std::system_category(), msg };
    }
#else
     std::uint32_t net_byte_order_addr = inet_addr(cp);
#endif // if defined(_WIN32) || defined(__CYGWIN__)

    return net_byte_order_addr;
}


int Socket(int domain, int type, int protocol)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    int sock = fsocket(domain, type, protocol);

    if (sock == INVALID_SOCKET) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(SOCKET) " };

        switch (error)
        {
        case WSAEPROTOTYPE:
            msg += "Unsupported protocol for this socket.";
            break;
        case WSAESOCKTNOSUPPORT:
            msg += "Unsupported address family for this type of socket";
            break;
        default:
            msg += "Failed to create a socket descriptor";
            break;
        }

        throw std::system_error{ error, std::system_category(), msg };
    }
#else
    int sock = socket(domain, type, protocol);
#endif // if defined(_WIN32) || defined(__CYGWIN__)

    return sock;
}


int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    auto result = fconnect(sockfd, addr, addrlen);

    // Interrupted function call
    while (result == SOCKET_ERROR && cpprerr::get_last_error() == WSAEINTR)
        result = fconnect(sockfd, addr, addrlen);

    if (result == SOCKET_ERROR) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(CONNECT) " };

        switch (error)
        {
        case WSAEADDRNOTAVAIL: 
            msg += "Invalid address; failed to connect";
            break;
        case WSAENETUNREACH: 
            msg += "Host network is unreachable";
            break;
        case WSAEHOSTUNREACH: 
            msg += "Host is unreachable";
            break;
        case WSAETIMEDOUT: 
            msg += "The connect function timed out";
            break;
        default:
            msg += "Error connecting to host";
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
int Send(int sockfd, const char* buffer, size_t len, int flags)
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

    return result;
}


int Recv(int sockfd, void* buffer, size_t len, int flags)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    // auto result = recv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);
    auto result = frecv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);

    while (result == -1 && cpprerr::get_last_error() == WSAEINTR)
        result = frecv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);
#else
    auto result = recv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);

    while (result == -1 && cpprerr::get_last_error() == EINTR)
        result = frecv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);

#endif // defined(_WIN32) || defined(__CYGWIN__)

    if (result == -1)
        throw std::system_error{ cpprerr::get_last_error(), std::system_category(), "Failed to read data" };

    return result;
}


int Close(int fd)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return fclosesocket(fd);
#else
	return close(fd);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


int Getaddrinfo(const char* node,
    const char* service,
    const struct addrinfo* hints,
    struct addrinfo** res)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    return fgetaddrinfo(node, service, hints, res);
#else
    return getaddrinfo(node, service, hints, res);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}


void Freeaddrinfo(struct addrinfo* ai)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    ffreeaddrinfo(ai);
#else
    freeaddrinfo(ai);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}