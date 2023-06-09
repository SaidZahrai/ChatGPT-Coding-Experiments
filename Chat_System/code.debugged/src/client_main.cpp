#include <chrono>
#include <thread>
#include <future>
#include <iostream>
#include <sstream>
#include "chat_client.h"

void messageReceiver(ChatClient& client) {
    std::mutex cout_mutex; // ChatGPT4 forgot!
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        auto messages = client.GetMessages();
        if (!messages.empty()) {
            std::lock_guard<std::mutex> lock(cout_mutex);
            for (const auto& message : messages) {
                std::cout << "Message from " << message.sender_id() << ": " << message.message() << std::endl;
            }
            std::cout << client.GetName() << " - ready::" << std::endl;
        }
    }
}

void messageSender(ChatClient& client) {
    std::mutex cout_mutex; // ChatGPT4 forgot!
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int receiver_id;
        char comma;
        std::string message;

        if (iss >> receiver_id >> comma && comma == ',' && std::getline(iss, message)) {
            std::lock_guard<std::mutex> lock(cout_mutex);
            if (receiver_id == 0) {
                auto clients = client.ListClients(); // ChatGPT made mistate and called client.GetClients()
                std::cout << "Registered clients: \n";
                for (const auto& client : clients) {
                    std::cout << "Client name: " << client.name() << ", Client ID: " << client.id() << "\n"; // GPT made mistake. Had to correct 
                }
            } else {
                bool success = client.SendMessage(receiver_id, message);
                if (success) {
                    std::cout << "Message sent successfully." << std::endl;
                } else {
                    std::cout << "Failed to send message." << std::endl;
                }
            }
            std::cout << client.GetName() << " - ready::" << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <client name>" << std::endl;
        return 1;
    }

    std::string client_name = argv[1];
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    ChatClient client(channel, client_name);

    std::cout << client.GetName() << " - ready::" << std::endl;

    std::thread receiverThread(messageReceiver, std::ref(client));
    std::thread senderThread(messageSender, std::ref(client));

    receiverThread.join();
    senderThread.join();

    return 0;
}
