// #include <grpcpp/grpcpp.h>
// #include "chat.grpc.pb.h"

// // class ChatClient {
// // public:
// //     ChatClient(std::shared_ptr<grpc::Channel> channel)
// //             : stub_(ChatService::NewStub(channel)) {}

// //     std::string SendMessage(const std::string& user, const std::string& message) {
// //         MessageRequest request;
// //         request.set_user(user);
// //         request.set_message(message);

// //         MessageResponse response;
// //         grpc::ClientContext context;
// //         grpc::Status status = stub_->SendMessage(&context, request, &response);

// //         if (status.ok()) {
// //             return response.status();
// //         } else {
// //             return "RPC failed";
// //         }
// //     }

// // private:
// //     std::unique_ptr<ChatService::Stub> stub_;
// // };

// // int main(int argc, char** argv) {
// //     ChatClient chat(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
// //     std::string user = "user1";
// //     std::string message = "Hello, world!";
// //     std::string reply = chat.SendMessage(user, message);
// //     std::cout << "Received: " << reply << std::endl;
// //     return 0;
// // }

// class ChatClient {
// public:
//     ChatClient(std::shared_ptr<grpc::Channel> channel) : stub_(Chat::NewStub(channel)) {}

//     std::string SendMessage(const std::string& user, const std::string& message) {
//         ChatRequest request;
//         request.set_user(user);
//         request.set_message(message);

//         ChatReply reply;

//         grpc::ClientContext context;

//         grpc::Status status = stub_->SendChat(&context, request, &reply);

//         if (status.ok()) {
//             return reply.message();
//         } else {
//             std::cout << "SendMessage RPC failed" << std::endl;
//             return "RPC failed";
//         }
//     }

// private:
//     std::unique_ptr<Chat::Stub> stub_;
// };

// int main(int argc, char** argv) {
//     ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
//     std::string user = "client1";
//     std::string text;
//     while (true) {
//         std::cout << "Enter your message: ";
//         std::getline(std::cin, text);
//         if (text == "exit") {
//             break;
//         }
//         std::string reply = client.SendMessage(user, text);
//         std::cout << "Server replied: " << reply << std::endl;
//     }
//     return 0;
// }

#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

class ChatClient {
public:
    ChatClient(std::shared_ptr<grpc::Channel> channel) : stub_(Chat::ChatService::NewStub(channel)) {}

    std::string SendMessage(const std::string& user, const std::string& message) {
        Chat::MessageRequest request;
        request.set_user(user);
        request.set_message(message);

         Chat::MessageResponse reply;

        grpc::ClientContext context;

        grpc::Status status = stub_->SendChat(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << "SendMessage RPC failed" << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Chat::ChatService::Stub> stub_;
};

int main(int argc, char** argv) {
    ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user = "client1";
    std::string text;
    while (true) {
        std::cout << "Enter your message: ";
        std::getline(std::cin, text);
        if (text == "exit") {
            break;
        }
        std::string reply = client.SendMessage(user, text);
        std::cout << "Server replied: " << reply << std::endl;
    }
    return 0;
}
