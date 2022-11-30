#include "socket_util.h"
#include "includes.h"


// TODO accept const std::string as host rather than const char
// TODO: Write to a std::string rather than char pointer?
// TODO: Return protocol used by host or -1 on error
//ssize_t lookup_host(const char *host, char* addr_buffer) {
//    struct addrinfo hints, *result;
//    int rtn_code;
//    void *address;
//
//    memset(&hints, 0, sizeof(hints));
//
//    /* IPv4 or IPv6 */
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//
//    /* Set ai_canonname field of the first addrinfo structure points to the official name of the host. */
//    hints.ai_flags |= AI_CANONNAME;
//
//    rtn_code = fgetaddrinfo(host, NULL, &hints, &result);
//    if (rtn_code != 0)
//        return -1;
//
//    /* result is a linked list of addrinfo structures; we use only the head node's data */
//    switch (result->ai_family) {
//        case AF_INET : address = &((struct sockaddr_in *)result->ai_addr)->sin_addr;
//            break;
//        case AF_INET6 : address = &((struct sockaddr_in6 *)result->ai_addr)->sin6_addr;
//            break;
//    }
//
//    inet_ntop(result->ai_family, address, addr_buffer, INET6_ADDRSTRLEN);
//
//    freeaddrinfo(result);
//    return 0;
//}
