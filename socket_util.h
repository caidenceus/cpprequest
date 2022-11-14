#pragma once

// TODO: investigate using std::string rather than char pointers as arguments
/**
 * @brief Resolve the IPv4 or IPv6 address of a domain name or IP address.
 *
 * Note:
 *     addr_buffer is to be of length INET6_ADDRSTRLEN defined in
 *     <netinet/in.h>.
 *
 * @param host The domain name or IP address to reslove.
 * @param addr_buffer The string buffer to write the IP address to.
 */
ssize_t lookup_host(const char *host, char* addr_buffer);
