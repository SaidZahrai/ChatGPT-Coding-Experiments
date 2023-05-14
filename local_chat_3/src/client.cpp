#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using chat::ChatService;
using chat::Message;

class ChatClient {
public:
    ChatClient(std::shared_ptr<Channel> channel) : stub_(ChatService::NewStub(channel)) {}

    void Chat() {
        ClientContext context;

        std::shared_ptr<grpc::ClientReaderWriter<Message, Message>> stream(stub_->Chat(&context));

        std::thread writer([stream]() {
            std::string text;
            while (std::getline(std::cin, text)) {
                Message message;
                message.set_user("Alice");
                message.set_text(text);

                stream->Write(message);
            }
            stream->WritesDone();
        });

        Message message;
        while (stream->Read(&message)) {
            std::cout << message.user() << ": " << message.text() << std::endl;
        }

        writer.join();
        grpc::Status status = stream->Finish();
        if (!status.ok()) {
            std::cout << "Chat rpc failed." << std::endl;
        }
    }

private:
    std::unique_ptr<ChatService::Stub> stub_;
};

int main(int argc, char** argv) {
    ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    client.Chat();

    return 0;
}
