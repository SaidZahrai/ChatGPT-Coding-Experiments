#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>

// Function to handle each client connection
void handleClient(int clientSocket) {
    // Send data to the client
    const char* message = "Hello, client!";
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        std::cerr << "Failed to send data to client" << std::endl;
    }

    // Close the client socket
    close(clientSocket);
}

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
    if (listen(serverSocket, 5) < 0) { // Set the maximum number of pending connections
        std::cerr << "Failed to listen" << std::endl;
        return -1;
    }

    std::cout << "Waiting for incoming connections..." << std::endl;

    std::vector<std::thread> clientThreads; // Store the client threads

    while (true) {
        // Accept incoming connections
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        std::cout << "Client connected!" << std::endl;

        // Create a new thread to handle the client connection
        std::thread clientThread(handleClient, clientSocket);

        // Detach the thread to allow it to run independently
        clientThread.detach();

        // Store the thread in the vector
        clientThreads.push_back(std::move(clientThread));
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
