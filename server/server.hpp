#pragma once

namespace net {
    /// A TCP Server
    class Server {
    private:
        unsigned int port;
        int sock_fd;
        std::string ip;
    public:
        Server(unsigned int port, const std::string& ip);
        void listen(int);
        int accept();
        ~Server();
    };

}