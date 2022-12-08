#pragma once

#include "definition.h"
#include "includes.h"
#include "socket_wrapper.h"


class HttpStream {
private:
    int sockfd;
    struct sockaddr_in serv_addr;

    std::string host;
    std::string port;

#if defined(_WIN32) || defined(__CYGWIN__)
    bool winsock_initialized;

    /**
     * @brief Initialize program for calling functions from Winsock.
     *
     * If on Windows, this function is called in the constructor and calls WSAStartup() 
     * as well as loads functions from ws2_lib.dll.
     */
    void winsock_init();
#endif // defined(_WIN32) || defined(__CYGWIN__)

public:
    /**
     * @brief Initialize an HttpStream object.
     *
     * The constructor is responsible for creating the socket descriptor that
     * will be used by this request, and initializing Winsock if on Windows.
     * 
     * @param uri The URI to export the host and or port from for the TCP connection.
     */
    HttpStream(cppr::Uri uri);

    /**
     * @breif Fill this->server_addr and establish the TCP three-way handshake.
     */
    void init();

    ssize_t data_stream(std::string write_buffer, char* read_buffer, size_t read_buff_size);

    /**
     * @brief Cleanup this HttpStream instance.
     *
     * Closes the socket descriptor and calls WSACleanup() if on Windows.
     */
    int close();
};


/**
 * @brief Read n bytes to a buffer from the socket descriptor sockfd.
 *
 * The purpose of this function is to handle the possibilities that
 * read may 1) encounter an EOF before reading n bytes, or 2) get
 * interrupted by a handled signal.
 *
 * @param sockfd The socket descriptor to read from.
 * @param buffer The buffer to write to.
 * @param n The number of bytes to read from the buffer to the socket descriptor.
 * @return The number of bytes read or -1 on error.
 */

ssize_t read_n_bytes(int sockfd, char* buffer, size_t n);
