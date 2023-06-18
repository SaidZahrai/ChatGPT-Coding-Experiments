#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Create a client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // Set up the server address
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set the server IP address here
    serverAddress.sin_port = htons(8080); // Set the server port number here

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }

    std::cout << "Connected to the server!" << std::endl;

    // Send and receive messages every 2 seconds
    while (true) {
        const char* message = "Hello, server!";
        if (send(clientSocket, message, strlen(message), 0) < 0) {
            std::cerr << "Failed to send data to server" << std::endl;
            break;
        }

        char buffer[1024];
        if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
            std::cerr << "Failed to receive data from server" << std::endl;
            break;
        }

        std::cout << "Server sent: " << buffer << std::endl;

        sleep(2); // Delay for 2 seconds before sending the next message
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
