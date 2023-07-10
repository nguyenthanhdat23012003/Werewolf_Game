//
// Created by nam on 4/23/23.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <set>

int main() {
    std::ios_base::sync_with_stdio(false);
    const int PORT = 8080;
    const int BACKLOG = 5;

    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (sockaddr * ) & address, sizeof(address)) == -1) {
        std::cerr << "Error binding socket to port" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        return 1;
    }

    std::set <int> clients;
    while (true) {
        // Accept incoming connection
        sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_fd = accept(server_fd, (sockaddr * ) & client_address, &client_address_len);
        clients.insert(client_fd);
        if (client_fd == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // Read HTTP request from client
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            std::cerr << "Error reading from socket" << std::endl;
            close(client_fd);
            continue;
        }

        // Parse HTTP request
        std::string request(buffer, bytes_read);
        size_t method_end = request.find(' ');
        if (method_end == std::string::npos) {
            std::cerr << "Invalid HTTP request" << std::endl;
            close(client_fd);
            continue;
        }
        std::string method = request.substr(0, method_end);
        if (method != "GET" && method != "POST") {
            std::cerr << "Unsupported HTTP method: " << method << std::endl;
            close(client_fd);
            continue;
        }

        // Log request to console
        std::cout << "Request:" << std::endl;
        std::cout << request << std::endl;

        // Send HTTP response to client
        std::string response;
        if (method == "GET") {
            response = "HTTP/1.1 200 OK\r\n\r\nHello, world!";
            if (write(client_fd, response.c_str(), response.size()) == -1) {
                std::cerr << "Error writing to socket" << std::endl;
            }
        } else if (method == "POST") {
            // Parse HTTP request headers to get content length
            size_t content_length_pos = request.find("Content-Length: ");
            if (content_length_pos == std::string::npos) {
                std::cerr << "Missing Content-Length header" << std::endl;
                close(client_fd);
                continue;
            }
            content_length_pos += 16;
            size_t content_length_end = request.find("\r\n", content_length_pos);
            if (content_length_end == std::string::npos) {
                std::cerr << "Invalid Content-Length header" << std::endl;
                close(client_fd);
                continue;
            }
            int content_length = std::stoi(request.substr(content_length_pos, content_length_end - content_length_pos));

            // Read HTTP request body from client
            std::string body;
            if (content_length > 0) {
                body.resize(content_length);
                body = request.substr(request.length() - content_length, content_length);
            }

            if (body.compare("start") == 0) {
                // Iterate over list of client file descriptors
                for (int client : clients) {
                    // Construct message to send
                    response = "HTTP/1.1 200 OK\r\n\r\nStart";

                    // Send message to client
                    int bytes_sent = write(client, response.c_str(), response.length());
                    if (bytes_sent == -1) {
                        std::cerr << "Error sending message to client" << std::endl;
                        continue;
                    }
                }

            }
            else {
                response = "HTTP/1.1 200 OK\r\n\r\nPOST request received";
                if (write(client_fd, response.c_str(), response.size()) == -1) {
                    std::cerr << "Error writing to socket" << std::endl;
                }
            }


        }
        // Close connection
        close(client_fd);
    }

    return 0;
}
