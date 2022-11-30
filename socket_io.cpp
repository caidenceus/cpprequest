#include "socket_io.h"
#include "socket_wrapper.h"
#include "loaddll.h"


ssize_t HttpStream::init() {
  // TODO: protocol agnostic
  this->sockfd = fsocket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, 0, sizeof(serv_addr));

  char domain_ip[INET6_ADDRSTRLEN];
  memset(&domain_ip, 0, sizeof(domain_ip));

  // TODO: Convert lookup_host to take host as std::string
  // lookup_host(this->host.c_str(), domain_ip);

  // TODO: protocol agnostic
  this->serv_addr.sin_family = AF_INET;
  this->serv_addr.sin_port = Htons(atoi(this->port.c_str()));

  this->serv_addr.sin_addr.s_addr = Inet_addr(this->host.c_str());

  // TODO: write socket wrapper functions to handle error checking
  if (Connect(this->sockfd, (struct sockaddr *) &(this->serv_addr), sizeof(this->serv_addr)) < 0) {
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


// TODO: error handling
ssize_t write_n_bytes(int sockfd, const std::string send_buff, size_t n) {
    int bytes_written{ 0 };
    bytes_written = fsend(sockfd, send_buff.c_str(), (int)send_buff.length(), 0);
    return bytes_written;
}


// TODO: error handling
ssize_t read_n_bytes(int sockfd, char* recv_buff, size_t n) {
    int bytes_rcvd{ 0 };
    do {
        bytes_rcvd = frecv(sockfd, recv_buff, n, 0);
    } while (bytes_rcvd > 0);
    return bytes_rcvd;
}

