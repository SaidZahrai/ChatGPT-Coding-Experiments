#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using chat::ChatService;
using chat::ChatRequest;
using chat::ChatReply;

class ChatClient {
public:
    ChatClient(std::shared_ptr<Channel> channel) : stub_(ChatService::NewStub(channel)) {}

    std::string SendMessage(const std::string& user, const std::string& message) {
        ChatRequest request;
        request.set_user(user);
        request.set_message(message);

        ChatReply reply;

        ClientContext context;

        Status status = stub_->SendMessage(&context, request, &reply);

        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<ChatService::Stub> stub_;
};

int main(int argc, char** argv) {
    ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user = "Alice";
    std::string message = "Hello World";
    std::string reply = client.SendMessage(user, message);
    std::cout << "Received: " << reply << std::endl;

    return 0;
}
