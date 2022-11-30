#include "socket_wrapper.h"

// TODO: add error handling to all wrapper functions


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
	return fconnect(sockfd, addr, addrlen);
#else
	return connect(sockfd, addr, addrlen);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
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
