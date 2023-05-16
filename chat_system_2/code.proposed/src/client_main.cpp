#include <chrono>
#include <thread>
#include <future>
#include <iostream>
#include <sstream>
#include "chat_client.h"

void messageReceiver(ChatClient& client) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
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
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int receiver_id;
        char comma;
        std::string message;

        if (iss >> receiver_id >> comma && comma == ',' && std::getline(iss, message)) {
            bool success = client.SendMessage(receiver_id, message);
            std::lock_guard<std::mutex> lock(cout_mutex);
            if (success) {
                std::cout << "Message sent successfully." << std::endl;
            } else {
                std::cout << "Failed to send message." << std::endl;
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
