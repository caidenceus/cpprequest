#include "config.h" // HTTP_BUFF_SIZE
#include "cpprequest.h"
#include "error.h"
#include "loaddll.h"
#include "socket_io.h"
#include "socket_wrapper.h"


HttpStream::HttpStream(cppr::Uri uri) : sockfd{ -1 }, serv_addr{  }, host{ uri.host }, port{ uri.port }
#if defined(_WIN32) || defined(__CYGWIN__)
    , winsock_initialized{ false }
#endif // defined(_WIN32) || defined(__CYGWIN__)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    this->winsock_init();
#endif // defined(_WIN32) || defined(__CYGWIN__)

    // TODO: Pass internet protocol to HttpStream ctor
    this->sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1)
    {
        this->close();
        throw std::system_error{ 
            cpprerr::get_last_error(), std::system_category(), "Failed to open socket fd" 
        };
    }
}


#if defined(_WIN32) || defined(__CYGWIN__)
void HttpStream::winsock_init()
{
    BOOL dlls_loaded = LoadDLLs();

    if (!dlls_loaded)
    {
        throw std::system_error{
            cpprerr::get_last_error(), std::system_category(), "Unable to load DLLs"
        };
    }

    WSADATA wsaData;
    const auto error = fWSAStartup(MAKEWORD(2, 2), &wsaData);
    if (error != 0)
        throw std::system_error{ error, std::system_category(), "WSAStartup failed" };

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        fWSACleanup();
        throw std::runtime_error{ "Invalid WinSock version" };
    }

    this->winsock_initialized = true;
}
#endif // defined(_WIN32) || defined(__CYGWIN__)


int HttpStream::close()
{
#if defined(_WIN32) || defined(__CYGWIN__)
    if (this->winsock_initialized)
        fWSACleanup();
#endif // defined(_WIN32) || defined(__CYGWIN__)
    if (this->sockfd != -1)
        return Close(this->sockfd);
    return 0;
}


void HttpStream::init()
{
    memset(&(this->serv_addr), 0, sizeof(this->serv_addr));

    char domain_ip[INET6_ADDRSTRLEN];
    memset(&domain_ip, 0, sizeof(domain_ip));

    // TODO: Convert lookup_host to take host as std::string
    // lookup_host(this->host.c_str(), domain_ip);

    // TODO: protocol agnostic
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = Htons(static_cast<uint16_t>(atoi(this->port.c_str())));

    this->serv_addr.sin_addr.s_addr = Inet_addr(this->host.c_str());

    if (Connect(this->sockfd, (struct sockaddr *) &(this->serv_addr), sizeof(this->serv_addr)) < 0)
    {
        throw std::system_error{
            cpprerr::get_last_error(), std::system_category(), "Socket failed to connect"
        };
    }
}


ssize_t HttpStream::data_stream(std::string write_buffer, char* read_buffer, size_t read_buff_size)
{
    int total, received, size;
    auto remaining = write_buffer.size();
    auto send_data = write_buffer.c_str();

    // send the request
    while (remaining > 0)
    {
        size = Send(this->sockfd, send_data, remaining, 0);
        if (size == -1)
        {
            this->close();
            cpprerr::SocketIoError{ "Socket error: Unable to write to the HTTP stream.\n" };
        }
        remaining -= size;
        send_data += size;
    }

    total = read_buff_size - 1;
    received = read_n_bytes(this->sockfd, read_buffer, (size_t)total);

    if (received == -1)
    {
        this->close();
        cpprerr::SocketIoError{ "Socket error: Unable to read from the HTTP stream.\n" };
    }

    if (received == total)
    {
        this->close();
        cpprerr::BufferOverflowError{
            "Success reading from socket, but not all data written to receive buffer."};
    }

  return 0;
}


// TODO: error handling
ssize_t read_n_bytes(int sockfd, char* recv_buff, size_t n)
{
    int bytes_rcvd{ 0 };
    do {
        bytes_rcvd = Recv(sockfd, recv_buff, n, 0);
    } while (bytes_rcvd > 0);
    return bytes_rcvd;
}

