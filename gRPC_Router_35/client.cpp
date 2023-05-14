#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "router.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using router::ClientInfo;
using router::ClientRegistrationResponse;
using router::ClientDirectory;
using router::MessageRequest;
using router::MessageResponse;
using router::RegistrationService;
using router::DirectoryService;
using router::RoutingService;

class RouterClient {
public:
  RouterClient(std::shared_ptr<Channel> channel)
      : registration_stub_(RegistrationService::NewStub(channel)),
        directory_stub_(DirectoryService::NewStub(channel)),
        routing_stub_(RoutingService::NewStub(channel)) {}

  void Register(const std::string& name) {
    ClientInfo client_info;
    client_info.set_name(name);

    ClientRegistrationResponse response;
    ClientContext context;

    Status status = registration_stub_->RegisterClient(&context, client_info, &response);
    if (status.ok()) {
      int32_t client_id = response.id();
      std::cout << "Registered successfully! Client ID: " << client_id << std::endl;
      clientId_ = client_id;
    } else {
      std::cout << "Registration failed. Error: " << status.error_message() << std::endl;
    }
  }

  void GetDirectory() {
    ClientInfo client_info;
    client_info.set_id(clientId_);

    ClientDirectory response;
    ClientContext context;

    Status status = directory_stub_->GetDirectory(&context, client_info, &response);
    if (status.ok()) {
      std::cout << "Client Directory:" << std::endl;
      for (const auto& client : response.clients()) {
        std::cout << "ID: " << client.id() << ", Name: " << client.name() << std::endl;
      }
    } else {
      std::cout << "Failed to get directory. Error: " << status.error_message() << std::endl;
    }
  }

  void SendMessage(int32_t receiverId, const std::string& message) {
    MessageRequest request;
    request.set_sender_id(clientId_);
    request.set_receiver_id(receiverId);
    request.set_message(message);

    MessageResponse response;
    ClientContext context;

    Status status = routing_stub_->SendMessage(&context, request, &response);
    if (status.ok()) {
      if (response.success()) {
        std::cout << "Message sent successfully!" << std::endl;
      } else {
        std::cout << "Failed to send message. Receiver not found." << std::endl;
      }
    } else {
      std::cout << "Failed to send message. Error: " << status.error_message() << std::endl;
    }
  }

  void StartMessaging() {
    std::string input;
    while (true) {
      std::cout << "Enter 'D' to get directory or provide receiver ID and message (e.g., '2 Hello'): ";
      std::getline(std::cin, input);

      if (input.empty()) {
        continue;
      }

      if (input == "D" || input == "d") {
        GetDirectory();
      } else {
        int32_t receiverId = 0;
        std::string message;

        try {
          size_t spacePos = input.find(' ');
          receiverId = std::stoi(input.substr(0, spacePos));
          message = input.substr(spacePos + 1);
        } catch (const std::exception& e) {
          std::cout << "Invalid input format. Please provide receiver ID and message." << std::endl;
          continue;
        }

        if (receiverId == 0) {
          // Request directory
          GetDirectory();
        } else {
          // Send message to receiver
          SendMessage(receiverId, message);
        }
      }
    }
  }

private:
  std::unique_ptr<RegistrationService::Stub> registration_stub_;
  std::unique_ptr<DirectoryService::Stub> directory_stub_;
  std::unique_ptr<RoutingService::Stub> routing_stub_;
  int32_t clientId_ = 0;
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <client_name>" << std::endl;
    return 1;
  }

  std::string clientName(argv[1]);
  if (clientName.empty()) {
    std::cerr << "Client name cannot be empty." << std::endl;
    return 1;
  }

  RouterClient routerClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  routerClient.Register(clientName);
  routerClient.StartMessaging();

  return 0;
}

