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
	return fconnect(sockfd, addr, addrlen);
#else
	return connect(sockfd, addr, addrlen);
#endif // if defined(_WIN32) || defined(__CYGWIN__)
}