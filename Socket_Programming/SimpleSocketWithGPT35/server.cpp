#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
#include <atomic>

std::atomic<int> clientCounter(0); // Counter to assign unique client IDs

// Function to handle each client connection
void handleClient(int clientSocket, const std::string& clientAddress, int clientID) {
    // Send and receive data
    while (true) {
        char buffer[1024];
        if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
            std::cerr << "Failed to receive data from client " << clientID << " at " << clientAddress << std::endl;
            break;
        }

        std::cout << "Client " << clientID << " at " << clientAddress << " sent: " << buffer << std::endl;

        // Modify the response to include the client ID
        std::string response = "Client " + std::to_string(clientID) + ": I got your message!";
        if (send(clientSocket, response.c_str(), response.length(), 0) < 0) {
            std::cerr << "Failed to send data to client " << clientID << " at " << clientAddress << std::endl;
            break;
        }
    }

    // Close the client socket
    close(clientSocket);
    std::cout << "Client " << clientID << " at " << clientAddress << " disconnected" << std::endl;
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
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
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
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        // Get the client's IP address and port number
        std::string clientIP = inet_ntoa(clientAddress.sin_addr);
        int clientPort = ntohs(clientAddress.sin_port);
        std::string clientAddressStr = clientIP + ":" + std::to_string(clientPort);

        // Assign a unique client ID
        int clientID = clientCounter.fetch_add(1) + 1;

        std::cout << "Client " << clientID << " at " << clientAddressStr << " connected" << std::endl;

        // Create a new thread to handle the client connection
        std::thread clientThread(handleClient, clientSocket, clientAddressStr, clientID);

        // Detach the thread to allow it to run independently
        clientThread.detach();

        // Store the thread in the vector
        clientThreads.push_back(std::move(clientThread));
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
