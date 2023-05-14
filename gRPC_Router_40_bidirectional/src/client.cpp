#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using chat::ChatService;
using chat::RegisterRequest;
using chat::RegisterResponse;
using chat::DirectoryRequest;
using chat::DirectoryResponse;
using chat::RouteRequest;
using chat::RouteResponse;

using chat::GetMessagesRequest;     // ChatGPT forgot to mention
using chat::GetMessagesResponse;    // ChatGPT forgot to mention

class ChatClient {
public:
  ChatClient(std::shared_ptr<Channel> channel)
      : stub_(ChatService::NewStub(channel)) {}

  int RegisterUser(const std::string& username) {
    RegisterRequest request;
    request.set_name(username);

    RegisterResponse response;
    ClientContext context;

    Status status = stub_->RegisterUser(&context, request, &response);

    if (status.ok()) {
      return response.id();
    } else {
      return -1; // Indicate failure
    }
  }

  std::map<int, std::string> GetDirectory(int id) {
    DirectoryRequest request;
    request.set_id(id);

    DirectoryResponse response;
    ClientContext context;

    Status status = stub_->GetDirectory(&context, request, &response);

    std::map<int, std::string> result;
    if (status.ok()) {
      for (const auto& user : response.users()) {
        result[user.id()] = user.name();
      }
    }
    return result;
  }

  bool SendMessage(int sender_id, int receiver_id, const std::string& message) {
    RouteRequest request;
    request.set_sender_id(sender_id);
    request.set_receiver_id(receiver_id);
    request.set_message(message);

    RouteResponse response;
    ClientContext context;

    Status status = stub_->RouteMessage(&context, request, &response);
    return status.ok() && response.success();
  }

  // Add this method to the ChatClient class:

  std::vector<RouteRequest> GetMessages(int id) {
    GetMessagesRequest request;
    request.set_id(id);

    GetMessagesResponse response;
    ClientContext context;

    Status status = stub_->GetMessages(&context, request, &response);

    std::vector<RouteRequest> result;
    if (status.ok()) {
      for (const auto& message : response.messages()) {
        result.push_back(message);
      }
    }
    return result;
  }
private:
  std::unique_ptr<ChatService::Stub> stub_;
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <username>\n";
    return 1;
  }

  std::string username = argv[1];

  ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  int id = client.RegisterUser(username);
  if (id < 0) {
    std::cerr << "Failed to register user\n";
    return 1;
  }

  std::cout << "Registered as user " << id << "\n";

  // And in your main loop:

  while (true) {
    std::cout << username << "> ";
    int receiver_id;
    std::string message;
    std::cin >> receiver_id;
    std::getline(std::cin, message);

    if (receiver_id == 0) {
      auto directory = client.GetDirectory(id);
      for (const auto& user : directory) {
        std::cout << user.first << ": " << user.second << "\n";
      }
    } else {
      std::cout << "Trying to send: " << message << " To: " << receiver_id << std::endl;
      bool success = client.SendMessage(id, receiver_id, message);
      if (!success) {
        std::cout << "Failed to send message\n";
      }
    }

    // Check for new messages
    auto messages = client.GetMessages(id);
    for (const auto& msg : messages) {
      std::cout << "Sender ID: " << msg.sender_id() << "\n";
      std::cout << msg.message() << "\n";
    }
  }

  return 0;
}
