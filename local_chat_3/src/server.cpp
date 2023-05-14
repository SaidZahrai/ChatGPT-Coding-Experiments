#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chat::ChatService;
using chat::Message;

class ChatServiceImpl final : public ChatService::Service {
    grpc::Status Chat(grpc::ServerContext* context, grpc::ServerReaderWriter<Message, Message>* stream) override {
        Message message;
        while (stream->Read(&message)) {
            std::cout << message.user() << ": " << message.text() << std::endl;

            std::string server_text;
            std::cout << "Input your message: ";
            std::getline(std::cin, server_text);

            Message server_message;
            server_message.set_user("Server");
            server_message.set_text(server_text);
            stream->Write(server_message);
        }
        return grpc::Status::OK;
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
