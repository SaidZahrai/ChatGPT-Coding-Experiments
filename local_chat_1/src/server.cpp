// #include <grpcpp/grpcpp.h>
// #include "chat.grpc.pb.h"

// class ChatServiceImpl final : public ChatService::Service {
//     grpc::Status SendMessage(grpc::ServerContext* context, const MessageRequest* request,
//                              MessageResponse* response) override {
//         // Implement your chat logic here
//         response->set_status("Message received by: " + request->user());
//         return grpc::Status::OK;
//     }
// };

// class ChatServer final {
// public:
//     void Run() {
//         std::string server_address("0.0.0.0:50051");

//         grpc::ServerBuilder builder;
//         builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//         builder.RegisterService(&service_);

//         std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
//         std::cout << "Server listening on " << server_address << std::endl;

//         server->Wait();
//     }

// private:
//     class Service final : public Chat::Service {
//     public:
//         grpc::Status SendChat(grpc::ServerContext* context, const ChatRequest* request, ChatReply* reply) override {
//             std::cout << "Received message from " << request->user() << ": " << request->message() << std::endl;
            
//             // The server sends a reply back to the client
//             reply->set_message("Message received by the server: " + request->message());

//             return grpc::Status::OK;
//         }
//     };

//     Service service_;
// };

// void RunServer() {
//     std::string server_address("0.0.0.0:50051");
//     // ChatServiceImpl service;
//     ChatServer service;

//     grpc::ServerBuilder builder;
//     builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//     builder.RegisterService(&service);
//     std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
//     server->Wait();
// }

// int main(int argc, char** argv) {
//     RunServer();
//     return 0;
// }

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

class ChatServiceImpl final : public Chat::Service {
    grpc::Status SendChat(grpc::ServerContext* context, const ChatRequest* request,
                          ChatReply* reply) override {
        std::cout << "Received message from " << request->user() << ": " << request->message() << std::endl;

        // The server sends a reply back to the client
        reply->set_message("Message received by the server: " + request->message());

        return grpc::Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ChatServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}
