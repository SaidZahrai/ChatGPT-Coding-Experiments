#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <thread>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);  // port 5000
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Failed to set IP address." << std::endl;
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return -1;
    }

    while (true) {
        const char *msg = "Hello, Server!";
        if (write(sockfd, msg, strlen(msg)) < 0) {
            std::cerr << "Failed to send message." << std::endl;
            break;
        }
        
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        int n = read(sockfd, buffer, sizeof(buffer) - 1);
        if (n < 0) {
            std::cerr << "Error reading from socket." << std::endl;
            break;
        } else if (n == 0) {
            std::cout << "Server closed connection" << std::endl;
            break;
        } else {
            std::cout << "Received message: " << buffer << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    close(sockfd);
    return 0;
}
