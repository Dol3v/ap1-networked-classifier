#include "common.hpp"
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <iostream>

void common::send_with_prelude(int sock_fd, const void *data, unsigned int length) {
    std::string header;
    std::stringstream ss(header);
    ss << "length:" << length << "\n";
    // packet specifying length of next one
    ssize_t sent_bytes = ::send(sock_fd, ss.str().c_str(), ss.str().size(), 0);
    if (sent_bytes < ss.width()) {
        throw std::logic_error("Couldn't send prelude packet");
    }
    // data packet
    sent_bytes = ::send(sock_fd, data, length, 0);
    if (sent_bytes < length) {
        throw std::logic_error("Couldn't sent data packet");
    }
}

int common::parse_prelude(const std::string &contents) {
    const static std::string length_prelude = "length:";
    auto without_length_prefix = contents.substr(length_prelude.size());
    return std::stoi(without_length_prefix);
}

std::string common::recv_until(int sock_fd, char end_of_data) {
    char curr;
    ssize_t read_bytes;
    std::string data;
    std::stringstream ss(data);

    while (true) {
        read_bytes = ::read(sock_fd, &curr, 1);
        if (read_bytes == -1) {
            throw std::logic_error("Couldn't read from socket");
        }

        if (curr == end_of_data) {
            break;
        }

        if (read_bytes == 0) {
            throw std::logic_error("Socket closed");
        }
        ss << curr;
    }
    return ss.str();
}

void common::file_upload(const std::string &source_path, int sock_fd) {

    std::ifstream file(source_path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        common::send_with_prelude(sock_fd, buffer.data(), buffer.size());
    }
//    // get file size
//    std::ifstream input_file(source_path, std::fstream::in);
//    std::istringstream buffer;
//    buffer << input_file.rdbuf();
//    std::cout << buffer.str() << std::endl;
}

void common::file_recv_to_path(const std::string &destination_path, int sock_fd, unsigned int buffer_length) {
    std::cout << "file_recv_to_path: " << destination_path << " sock fd: " << sock_fd << std::endl;
    std::ofstream dest_file(destination_path, std::ios::binary | std::fstream::out);
    common::file_recv(dest_file, sock_fd, buffer_length);
    dest_file.close();
}

void common::file_recv(std::ofstream &dest_file, int sock_fd, unsigned int buffer_length) {
    std::cout << "file_recv" << std::endl;
    char buffer[buffer_length];
    std::string prelude_packet = common::recv_until(sock_fd, '\n');
    std::cout << prelude_packet << std::endl;
    unsigned int content_length_to_read = common::parse_prelude(prelude_packet);
    std::cout << content_length_to_read << std::endl;
    ssize_t read_bytes = 0;

    while (true) {
        read_bytes = ::read(sock_fd, buffer, buffer_length);
        if (read_bytes == buffer_length) {
            content_length_to_read -= buffer_length;
            dest_file.write(buffer, buffer_length);
        } else if (read_bytes == content_length_to_read) {
            dest_file.write(buffer, read_bytes);
            break;
        } else {
            throw std::logic_error("Couldn't read received content into file");
        }
    }
}
