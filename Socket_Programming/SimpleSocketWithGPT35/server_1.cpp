#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Create a server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // Bind the server socket to an IP address and port
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set your desired IP address here
    serverAddress.sin_port = htons(8080); // Set your desired port number here

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Failed to listen" << std::endl;
        return -1;
    }

    std::cout << "Waiting for incoming connections..." << std::endl;

    // Accept incoming connections
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept connection" << std::endl;
        return -1;
    }

    std::cout << "Client connected!" << std::endl;

    // Send data to the client
    const char* message = "Hello, client!";
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        std::cerr << "Failed to send data to client" << std::endl;
        return -1;
    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
