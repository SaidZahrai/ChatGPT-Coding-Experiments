#include <iostream>
#include <memory>
#include <unordered_map>
#include <grpcpp/grpcpp.h>
#include "router.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using router::ClientInfo;
using router::ClientRegistrationResponse;
using router::ClientDirectory;
using router::MessageRequest;
using router::MessageResponse;
using router::RegistrationService;
using router::DirectoryService;
using router::RoutingService;

class RouterServiceImpl final : public RegistrationService::Service,
                               public DirectoryService::Service,
                               public RoutingService::Service {
  std::unordered_map<int32_t, std::string> directory;

  int32_t generateClientId() {
    static int32_t nextId = 1;
    return nextId++;
  }

  Status RegisterClient(ServerContext* context, const ClientInfo* request,
                        ClientRegistrationResponse* response) override {
    int32_t clientId = generateClientId();
    response->set_id(clientId);
    directory[clientId] = request->name();
    return Status::OK;
  }

  Status GetDirectory(ServerContext* context, const ClientInfo* request,
                      ClientDirectory* response) override {
    for (const auto& entry : directory) {
      ClientInfo* client = response->add_clients();
      client->set_id(entry.first);
      client->set_name(entry.second);
    }
    return Status::OK;
  }

  Status SendMessage(ServerContext* context, const MessageRequest* request,
                     MessageResponse* response) override {
    int32_t senderId = request->sender_id();
    int32_t receiverId = request->receiver_id();
    const std::string& message = request->message();

    if (directory.count(receiverId) == 0) {
      response->set_success(false);
    } else {
      std::cout << "Message received: Sender ID: " << senderId
                << ", Receiver ID: " << receiverId << ", Message: " << message
                << std::endl;
      response->set_success(true);
    }
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  RouterServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
 
  server->Wait();
}

int main() {
  RunServer();
  return 0;
}

