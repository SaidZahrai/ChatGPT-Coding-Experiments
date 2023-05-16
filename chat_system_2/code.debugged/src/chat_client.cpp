#include "chat_client.h"

ChatClient::ChatClient(std::shared_ptr<grpc::Channel> channel, const std::string& name)
    : stub_(ChatService::NewStub(channel)) {
  RegisterClientRequest request;
  request.set_name(name);
  
  RegisterClientResponse response;
  grpc::ClientContext context;

  grpc::Status status = stub_->RegisterClient(&context, request, &response);
  if (status.ok()) {
    id = response.id();
    if (id == -1) {
      throw std::runtime_error("Registration failed, name already taken.");
    }
  } else {
    throw std::runtime_error("Registration failed, server unavailable.");
  }
}

std::vector<Client> ChatClient::ListClients() {
  grpc::ClientContext context;
  ListClientsRequest request;
  ListClientsResponse response;

  grpc::Status status = stub_->ListClients(&context, request, &response);
  if (status.ok()) {
    std::vector<Client> clients(response.clients().begin(), response.clients().end());
    return clients;
  } else {
    throw std::runtime_error("Failed to get list of clients.");
  }
}

bool ChatClient::SendMessage(int receiver_id, const std::string& message) {
  grpc::ClientContext context;
  SendMessageRequest request;
  request.set_sender_id(id);
  request.set_receiver_id(receiver_id);
  request.set_message(message);

  SendMessageResponse response;

  grpc::Status status = stub_->SendMessage(&context, request, &response);
  return status.ok();
}

std::vector<Message> ChatClient::GetMessages() {
  grpc::ClientContext context;
  GetMessagesRequest request;
  request.set_client_id(id);
  
  GetMessagesResponse response;

  grpc::Status status = stub_->GetMessages(&context, request, &response);
  if (status.ok()) {
    std::vector<Message> messages(response.messages().begin(), response.messages().end());
    return messages;
  } else {
    throw std::runtime_error("Failed to get messages.");
  }
}

std::string ChatClient::GetName() { // ChatGPT4 forgot
  return name_;
}
