#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>

void handle_client(int newsockfd) {
    char buffer[256];
    memset(buffer, 0, 256);
    int n = read(newsockfd, buffer, 255);
    if (n < 0) {
        std::cerr << "Error reading from socket." << std::endl;
    } else {
        std::cout << "Here is the message: " << buffer << std::endl;
    }
    close(newsockfd);
}

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

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Failed to bind to address." << std::endl;
        return -1;
    }

    listen(sockfd, 5);

    while (true) {
        socklen_t clilen;
        struct sockaddr_in cli_addr;
        clilen = sizeof(cli_addr);

        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            std::cerr << "Failed to accept connection." << std::endl;
            continue;
        }

        std::thread client_thread(handle_client, newsockfd);
        client_thread.detach();
    }

    close(sockfd);
    return 0;
}
