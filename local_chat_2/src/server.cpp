#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chat::ChatService;
using chat::ChatRequest;
using chat::ChatReply;

class ChatServiceImpl final : public ChatService::Service {
    Status SendMessage(ServerContext* context, const ChatRequest* request, ChatReply* reply) override {
        std::string prefix = "Hello " + request->user();
        reply->set_message(prefix);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ChatServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}
