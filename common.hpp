#pragma once

#include <string>

#define PORT 5000

/// Utility functions that are common to both the client and server.
namespace common {

    /// Sends data over the socket with descriptor sock_fd, with a prelude packet specifying the length of the data.
    void send_with_prelude(int sock_fd, const void* data, unsigned int length);

    /// Receives data from the socket until a specific char is received.
    std::string recv_until(int sock_fd, char end_of_data);

    /// Uploads a file over the specified socket.
    void file_upload(const std::string& source_path, int sock_fd);

    /// Receives a file over the specified socket, and saves it to the given path.
    void file_recv_to_path(const std::string& destination_path, int sock_fd, unsigned int buffer_length = 4096);

    /// Receives a file over the specified socket.
    void file_recv(std::ofstream& dest_file, int sock_fd, unsigned int buffer_length = 4096);

    /// Parses prelude packet and returns the content's length.
    int parse_prelude(const std::string& contents);
}