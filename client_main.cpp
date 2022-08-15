#include <iostream>
#include "client/client.hpp"
#include "common.hpp"

#define SERVER_IP "127.0.0.1"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " /path/to/unclassified /path/to/classified" << std::endl;
        return 1;
    }

    // connect to server and send message
    net::Client client(SERVER_IP, PORT);
    common::file_upload(argv[1], client.sock_fd);

    // recv response from server and save
    common::file_recv_to_path(argv[2], client.sock_fd);
}