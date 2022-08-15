#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include "server.hpp"

net::Server::Server(unsigned int port, const std::string& ip) : ip(ip), port(port) {
    this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sock_fd == 0) {
        throw std::logic_error("Couldn't create a socket");
    }

    struct sockaddr_in address{};
    memset(&address, 0, sizeof(address));

    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    address.sin_family = AF_INET;

    if (bind(this->sock_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) < 0) {
        throw std::logic_error("Couldn't bind the socket to the requested ip/port");
    }
}

void net::Server::listen(int backlog) {
    if (::listen(this->sock_fd, backlog) < 0) {
        throw std::logic_error("Couldn't listen for incoming connections");
    }
}

int net::Server::accept() {
    struct sockaddr_in accepted{};
    unsigned int accepted_len = 0;
    int accepted_fd = ::accept(this->sock_fd, reinterpret_cast<sockaddr*>(&accepted), &accepted_len);
    if (accepted_fd < 0 | accepted_len < sizeof(struct sockaddr)) {
        throw std::logic_error("Couldn't accept incoming connection");
    }
    return accepted_fd;
}


net::Server::~Server() {
    if (this->sock_fd) {
        ::shutdown(this->sock_fd, ::SHUT_RDWR);
    }
}

