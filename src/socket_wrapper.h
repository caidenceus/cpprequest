#ifndef CPPR_SOCKET_WRAPPER_H__
#define CPPR_SOCKET_WRAPPER_H__

#include <cstdint>

#if defined(_WIN32) || defined(__CYGWIN__)
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#include <winsock2.h>
#include <ws2tcpip.h> // socklen_t
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#else
#include <sys/socket.h>
#endif // defined(_WIN32) || defined(__CYGWIN__)

/**
 * @brief Converts an unsigned short integer from host byte order to network byte order. 
 *
 * Note: For more information see:
 *       Linux:   https://man7.org/linux/man-pages/man3/htons.3p.html
 *       Windows: https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-htons
 *
 * @param hostshort The unsigned host byte order integer to convert.
 * @return The network byte order representation of hostshort.
 */
std::uint16_t Htons(std::uint16_t hostshort);

/**
 * @brief Initiate a connection on a socket.
 *
 * Note: For more information see:
 *       Linux:   https://man7.org/linux/man-pages/man2/connect.2.html
 *       Windows: https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect
 *
 * @param sockfd The socket opened by the Socket function.
 * @param addr The address to connect sockfd to.
 * @param addrlen The size of addr.
 * @return 0 on success, -1 on error.
 */
int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

/**
 * @brief Write data to a socket descriptor.
 *
 * Note: The send() call may be used only when the socket is in a
 *       connected state (so that the intended recipient is known). The
 *       only difference between send() and write() is the presence of
 *       flags. With a zero flags argument, send() is equivalent to
 *       write().
 *
 * Note: For the flags arguement see:
 *       Linux:   https://man7.org/linux/man-pages/man2/send.2.html
 *       Windows: https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send
 *
 * @param sockfd The socket to write to.
 * @param buf The message to send.
 * @param len The size of buf.
 * @param flags The bitwise OR of zero or more send flags flags
 * @return On success, the number of bytes sent, -1 on error.
 */
int Send(int sockfd, const char* buf, size_t len, int flags);

/**
 * @brief Receive a message from a socket.
 *
 * Note: For the flags arguement, see
 *       Linux:   https://man7.org/linux/man-pages/man2/recv.2.html
 *       Windows: https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recv
 *
 * @param sockfd The socket to receive a message from.
 * @param buf The string to read the message into.
 * @param len The size of buf.
 * @param flags The flags argument is formed by ORing one or more recv flags.
 */
int Recv(int sockfd, void* buf, size_t len, int flags);

/**
 * @brief Create an endpoint for netowrk communication.
 *
 * Note: For arguement lists see:
 *       Linux:   https://man7.org/linux/man-pages/man2/socket.2.html
 *       Windows: https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
 *
 * @param domain The communication domain; this selects the protocol
 *               family which will be used for communication.
 * @param type The communication semantics.
 * @param protocol The communication protocol to use on the socket.
 * @return On success, a file descriptor for the new socket is returned, -1 on error.
 */
int Socket(int domain, int type, int protocol);

/**
 * @brief Close a file descriptor.
 *
 * @param fd The descriptor to close.
 * @return 0 on success, -1 on error.
 */
int Close(int fd);

/**
 * @brief Get a linked list of addrinfo structures filtered by hints.
 *
 * The addrinfo structure used by getaddrinfo() contains the
 * following fields:
 *
 *          struct addrinfo {
 *              int              ai_flags;
 *              int              ai_family;
 *              int              ai_socktype;
 *              int              ai_protocol;
 *              socklen_t        ai_addrlen;
 *              struct sockaddr *ai_addr;
 *              char            *ai_canonname;
 *              struct addrinfo *ai_next;
 *          };
 *
 * @param node A domain name or an IPv4 or IPv6 address.
 * @param service A port number or service name (i.e. "http").
 * @param hints An addrinfo structure used to filter the results.
 * @param res The pointer to the head of the addrinfo linked list.
 */
int Getaddrinfo(const char* node,
    const char* service,
    const struct addrinfo* hints,
    struct addrinfo** res);

/**
 * @brief Free memory used by linked list created from Getaddrinfo.
 */
void Freeaddrinfo(struct addrinfo* ai);

#endif // CPPR_SOCKET_WRAPPER_H__