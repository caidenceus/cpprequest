// TODO: remove adjunct headers
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <iostream> // debug

#include "error.h"
#include "socket_io.h"
#include "socket_util.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <cctype>
#include <climits>
#include <condition_variable>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <resolv.h>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif



// windows using socket_t = SOCKET;
// linuc
using socket_t = int;

ssize_t HttpStream::init() {
  // TODO: protocol agnostic
  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&serv_addr, sizeof(serv_addr));

  char domain_ip[INET6_ADDRSTRLEN];
  bzero(&domain_ip, sizeof(domain_ip));

  // TODO: Convert lookup_host to take host as std::string
  lookup_host(this->host.c_str(), domain_ip);

  // TODO: protocol agnostic
  this->serv_addr.sin_family = AF_INET;
  this->serv_addr.sin_port = htons(atoi(this->port.c_str()));

  this->serv_addr.sin_addr.s_addr = inet_addr(domain_ip);

  // TODO: write socket wrapper functions to handle error checking
  if (connect(this->sockfd, (struct sockaddr *) &(this->serv_addr), sizeof(this->serv_addr)) < 0) {
    std::cout << "ERROR connecting";
    return -1;
  }
  return 0;
}


ssize_t HttpStream::data_stream(std::string write_buffer, char* read_buffer, size_t read_buff_size) {
  int total, received, error;
  error = write_n_bytes(this->sockfd, write_buffer.c_str(), strlen(write_buffer.c_str()));

  if (error == -1) {
    cppr::error::SocketIoError{ "Socket error: Unable to write to the HTTP stream.\n" };
    return -1;
  }

  total = read_buff_size - 1;
  error = received = read_n_bytes(this->sockfd, read_buffer, total);

  if (error == -1) {
    cppr::error::SocketIoError{ "Socket error: Unable to read from the HTTP stream.\n" };
    return -1;
  }

  if (received == total) {
    cppr::error::BufferOverflowError{
      "Success reading from socket, but not all data written to receive buffer."};
    return -1;
  }

  return 0;
}


ssize_t write_n_bytes(int sockfd, const char* buffer, size_t n) {
  int b_left = n;
  int b_written = 0;
  int bytes;
  char* mutable_buffer = strdup(buffer);

  /* Break before b_left == 0. This way we do not have to check for write
   * returning 0, which only happens when we pass 0 as the number of 
   * bytes to write. Write returning 0 represents bad coding practice.
   */
  while (b_left > 0) {
    bytes = write(sockfd, mutable_buffer, b_left);

    if (bytes == -1) {
      /* Write was interrupted by a caught signal, call write again. */
      if (errno == EINTR) {
        bytes = 0;
      }
      else {
          return -1;
      }
    }

    b_left -= bytes;
    mutable_buffer += bytes;
  }

  return b_written;
}


ssize_t read_n_bytes(int sockfd, char* buffer, size_t n) {
  int b_left = n;
  int bytes;
  char* mutable_buffer = buffer;

  while (b_left > 0) {
    bytes = read(sockfd, mutable_buffer, b_left);

    if (bytes == -1) {
    /* Read was interrupted by a caught signal, call read again. */
      if (errno == EINTR) {
        bytes = 0;
      }
      else {
        return -1;
      }
    }
    /* EOF */
    else if (bytes == 0) {
      break;
    }

      b_left -= bytes;
      mutable_buffer += bytes;
  }

  /* b_left may be nonzero if we encountered an EOF */
  return (n - b_left);
}

