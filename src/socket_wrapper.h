#ifndef CPPR_SOCKET_WRAPPER_H__
#define CPPR_SOCKET_WRAPPER_H__

#include <cstdint>

#if defined(_WIN32) || defined(__CYGWIN__)
#include <winsock2.h>
#include <ws2tcpip.h> // socklen_t
#else
#include <sys/socket.h>
#endif

std::uint16_t Htons(std::uint16_t hostshort);

std::uint32_t Inet_addr(const char* cp);

int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

int Send(int sockfd, const char* buf, size_t len, int flags);

int Recv(int sockfd, void* buf, size_t len, int flags);

int Socket(int domain, int type, int protocol);

int Close(int fd);

#endif // CPPR_SOCKET_WRAPPER_H__