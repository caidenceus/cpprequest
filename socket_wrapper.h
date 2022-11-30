#pragma once

#include "cpprequest.h"

uint16_t Htons(uint16_t hostshort);

in_addr_t Inet_addr(const char* cp);

int Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

ssize_t Send(int sockfd, const char* buf, size_t len, int flags);

ssize_t Recv(int sockfd, char* buf, size_t len, int flags);