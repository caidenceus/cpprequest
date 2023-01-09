#include "socket_wrapper.h"

#include "error.h"    // get_last_error

#include <cstdint>
#include <iostream>

#if defined(_WIN32) || defined(__CYGWIN__)
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#include <winsock2.h>
#include <ws2tcpip.h> // socklen_t
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#include "loaddll.h"  // fhtons, finet_addr, fsocket, fconnect, fsend, frecf, fclosesocket
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif // defined(_WIN32) || defined(__CYGWIN__)


std::uint16_t Htons(std::uint16_t hostshort)
{
#if defined(_WIN32) || defined(__CYGWIN__)
	return fhtons(hostshort);
#else
	return htons(hostshort);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
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
        case WSAENETDOWN:
            msg += "Network service provider has failed.";
            break;
        default:
            msg += "Failed to create a socket descriptor";
            break;
        }

        fWSACleanup();
        throw std::system_error{ error, std::system_category(), msg };
    }
#else
    int sock = socket(domain, type, protocol);

    if (sock == -1) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(SOCKET) " };

        switch (error)
        {
        case EACCES:
            msg += "Permission to create socket denied.";
            break;
        default:
            msg += "Failed to create a socket descriptor";
            break;
        }

        fWSACleanup();
        throw std::system_error{ error, std::system_category(), msg };
    }
#endif // if defined(_WIN32) || defined(__CYGWIN__)

    return sock;
}


int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    // TODO: handle nonblocking sockets using select
    auto result = fconnect(sockfd, addr, addrlen);

    // Interrupted function call
    while (result == SOCKET_ERROR && cpprerr::get_last_error() == WSAEINTR)
        result = fconnect(sockfd, addr, addrlen);

    if (result == SOCKET_ERROR) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(CONNECT) " };

        switch (error)
        {
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

        fWSACleanup();
        throw std::system_error{ error, std::system_category(), msg };
    }
#else
    auto result = connect(sockfd, addr, addrlen);

    while (result == -1 && errno == EINTR)
        result = connect(sockfd, addr, addrlen);

    if (result == -1) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(CONNECT) " };

        switch (error)
        {
        case ENETUNREACH:
            msg += "Host network is unreachable";
            break;
        case ETIMEDOUT:
            msg += "The connect function timed out";
            break;
        default:
            msg += "Error connecting to host";
            break;
        }

        throw std::system_error{ error, std::system_category(), msg };
    }
#endif // defined(_WIN32) || defined(__CYGWIN__)

    return result;
}


// TODO: buf should be void instead of char
int Send(int sockfd, const char* buffer, size_t len, int flags)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    // auto result = fsend(sockfd, reinterpret_cast<const char*>(buffer), static_cast<int>(length), 0);
    auto result = fsend(sockfd, buffer, static_cast<int>(len), flags);

    while (result == SOCKET_ERROR && cpprerr::get_last_error() == WSAEINTR)
        result = fsend(sockfd, buffer, static_cast<int>(len), flags);

    if (result == SOCKET_ERROR) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(SEND) " };

        switch (error)
        {
        case WSAENETDOWN:
            msg += "The network subsystem is down.";
            break;
        case WSAEFAULT:
            msg += "The buf parameter is not completely contained in a valid part of the user address space.";
            break;
        case WSAENETRESET:
            msg += "The connection was broken";
            break;
        case WSAEHOSTUNREACH:
            msg += "The destination host is unreachable.";
            break;
        case WSAETIMEDOUT:
            msg += "The connection has been dropped.";
            break;
        default:
            msg += "Error sending data.";
            break;
        }

        fWSACleanup();
        throw std::system_error{ error, std::system_category(), msg };
    }
#else
    auto result = send(sockfd, buffer, len, flags);

    while (result == -1 && errno == EINTR)
        result = send(sockfd, buffer, len, flags);
    
    if (result == -1) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(SEND) " };

        switch (error)
        {
        case EACCES:
            msg += "Access denied.";
            break;
        case ECONNRESET:
            msg += "Connection reset by peer.";
            break;
        default:
            msg += "Error sending data.";
            break;
        }

        throw std::system_error{ error, std::system_category(), msg };
    }
#endif // defined(_WIN32) || defined(__CYGWIN__)

    return result;
}


int Recv(int sockfd, void* buffer, size_t len, int flags)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    // auto result = recv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);
    auto result = frecv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);

    while (result == -1 && cpprerr::get_last_error() == WSAEINTR)
        result = frecv(sockfd, reinterpret_cast<char*>(buffer), static_cast<int>(len), flags);

    if (result == INVALID_SOCKET) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(RECV) " };

        switch (error)
        {
        case WSAENETDOWN:
            msg += "The network subsystem is down.";
            break;
        case WSAEFAULT:
            msg += "Invalid receive buffer.";
            break;
        default:
            msg += "Error receiving data.";
            break;
        }

        fWSACleanup();
        throw std::system_error{ error, std::system_category(), msg };
    }
#else
    auto result = recv(sockfd, reinterpret_cast<char*>(buffer), len, flags);

    while (result == -1 && cpprerr::get_last_error() == EINTR)
        result = recv(sockfd, reinterpret_cast<char*>(buffer), len, flags);
    
    if (result == -1) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(RECV) " };

        switch (error)
        {
        case ECONNREFUSED:
            msg += "Remote connection refused.";
            break;
        case EFAULT:
            msg += "Invalid receive buffer.";
            break;
        default:
            msg += "Error receiving data.";
            break;
        }

        throw std::system_error{ error, std::system_category(), msg };
    }

#endif // defined(_WIN32) || defined(__CYGWIN__)

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
    int ret;
#if defined(_WIN32) || defined(__CYGWIN__)
    if ((ret = fgetaddrinfo(node, service, hints, res)) != 0) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(GETADDRINFO) " };

        switch (error)
        {
        case WSAEAFNOSUPPORT:
            msg += "Invalid value passed as ai_flamily in hints.";
            break;
        case WSANO_RECOVERY:
            msg += "Unknown unrecoverable error.";
            break;
        case WSATYPE_NOT_FOUND:
            msg += "The service is not supported by the ai_socktype member of hints.";
            std::cout << "The port provided appears to be closed.";
            break;
        case WSAESOCKTNOSUPPORT:
            msg += "The ai_socktype member of the hints parameter is not supported.";
            std::cout << "TCP is not supported on the host/port pair provided.";
            break;
        default:
            msg += "Error getting host information.";
            break;
        }

        fWSACleanup();
        throw std::system_error{ error, std::system_category(), msg };
    }
#else
    if ((ret = getaddrinfo(node, service, hints, res)) != 0) {
        const auto error = cpprerr::get_last_error();
        std::string msg{ "(GETADDRINFO) " };

        switch (error)
        {
        case EAI_ADDRFAMILY:
            msg += "The specified network host does not have any network addresses in the requested address family.";
            break;
        case EAI_FAIL:
            msg += "The name server returned a permanent failure indication..";
            break;
        case EAI_NONAME:
            msg += "The host or port is unknown.";
            std::cout << "The host or port is unknown.";
            break;
        case EAI_SERVICE:
            msg += "The requested service is not available for the requestedsocket type.";
            std::cout << "TCP is not supported on the host/port pair provided.";
            break;
        default:
            msg += "Error getting host information.";
            break;
        }

        throw std::system_error{ error, std::system_category(), msg };
    }
#endif // if defined(_WIN32) || defined(__CYGWIN__)

    return ret;
}


void Freeaddrinfo(struct addrinfo* ai)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    ffreeaddrinfo(ai);
#else
    freeaddrinfo(ai);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}
