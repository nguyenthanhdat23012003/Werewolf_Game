#include "RequestHelper.h"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 1024

RequestHelper::RequestHelper() {
    // Create TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Failed to set server address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to the server");
        exit(EXIT_FAILURE);
    }
}

RequestHelper::~RequestHelper() {
    // Close the socket
    close(sockfd);
}

std::string RequestHelper::sendRequest(const std::string& request) {
    std::string headers = "Content-Type: text/plain\r\n"
                          "Content-Length: " + std::to_string(request.length()) + "\r\n\r\n";
    std::string full_request = headers + request;

    // Send the request to the server
    if (send(sockfd, full_request.c_str(), full_request.length(), 0) < 0) {
        perror("Failed to send data");
        exit(EXIT_FAILURE);
    }

    // Receive the response from the server
    char buffer[MAX_BUFFER_SIZE];
    int recv_len;
    recv_len = recv(sockfd, buffer, sizeof(buffer), 0);
    if (recv_len < 0) {
        perror("Failed to receive data");
        exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0';
    return std::string(buffer);
}
