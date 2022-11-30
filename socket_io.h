#pragma once

#include "includes.h"


class HttpStream {
  private:
    int sockfd;
    struct sockaddr_in serv_addr;

    std::string host;
    std::string port;

  public:
    HttpStream(cppr::Uri uri) : host{uri.host}, port{uri.port} { ; }

    // Create the sockfd and fill out serv_addr
    ssize_t init();
    ssize_t data_stream(std::string write_buffer, char* read_buffer, size_t read_buff_size);
};


/**
 * @brief Write n bytes of a buffer to the socket descriptor sockfd.
 *
 * The purpose of this function is to handle possibility that write
 * will return less bytes than count due to insufficient space on the
 * physical medium, or if write is interrupted by a signal handler
 * after writing less than count bytes, for example.
 *
 * In the case of a partial write, the caller can invoke write again
 * to transfer the remaining bytes; this function is designed to
 * detect partial writes and call write again to transfer all bytes.
 *
 * Write is called internally and handled appropriately.
 * ssize_t write(int fd, const void *buf, size_t count);
 *
 * @param sockfd The socket descriptor to write to.
 * @param buffer The buffer to write from.
 * @param n The number of bytes to write from the buffer to the socket descriptor.
 * @return The number of bytes written or -1 on error.
 */
ssize_t write_n_bytes(int sockfd, const std::string send_buff, size_t n);


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
