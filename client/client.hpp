#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <string>

namespace net {
    /// TCP Client.
    class Client {
    private:
        uint16_t port;
        std::string server_ip;
        int conn_fd;
    public:
        Client(const std::string& server_ip, uint16_t port);
        ~Client();

        int sock_fd;
    };
}