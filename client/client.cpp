#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include "client.hpp"

net::Client::Client(const std::string& server_ip, uint16_t port) {
    if ((this->sock_fd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::logic_error("Couldn't create socket");
    }

    struct sockaddr_in server_address{};
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_port = ::htons(port);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());

    if ((this->conn_fd = ::connect(this->sock_fd, reinterpret_cast<struct sockaddr*>(&server_address),
            sizeof(server_address))) < 0) {
        throw std::logic_error("Failed to connect to server");
    }
}

net::Client::~Client() {
    if (this->sock_fd) {
        ::close(this->sock_fd);
    }
    if (this->conn_fd) {
        ::close(this->conn_fd);
    }
}
