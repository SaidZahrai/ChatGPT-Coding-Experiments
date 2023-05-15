#include <iostream>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using chat::ChatService;
using chat::RegisterRequest;
using chat::RegisterResponse;
using chat::DirectoryRequest;
using chat::DirectoryResponse;
using chat::RouteRequest;
using chat::RouteResponse;
using chat::User;

using chat::GetMessagesRequest;     // ChatGPT forgot to mention
using chat::GetMessagesResponse;    // ChatGPT forgot to mention

class ChatServiceImpl final : public ChatService::Service {
   public:
      Status RegisterUser(ServerContext* context, const RegisterRequest* request,
                     RegisterResponse* reply) override {
         // generate unique id and register user
         // omitting error handling for brevity
         int id = users.size() + 1;
         users[id] = request->name();
         reply->set_id(id);
         return Status::OK;
      }

      Status GetDirectory(ServerContext* context, const DirectoryRequest* request,
                     DirectoryResponse* reply) override {
         for (const auto& user : users) {
            User* user_proto = reply->add_users();
            user_proto->set_id(user.first);
            user_proto->set_name(user.second);
         }
         return Status::OK;
      }

      Status RouteMessage(ServerContext* context, const RouteRequest* request,
                        RouteResponse* response) override {
         std::lock_guard<std::mutex> lock(mu_);

         int receiver_id = request->receiver_id();
         std::cout << "RouteMessage tries to find receiver " << receiver_id << ".\n";
         // auto it = directory_.find(receiver_id);       ChatGPT mixed with users
         // if (it == directory_.end()) {                 ChatGPT mixed with users
         auto it = users.find(receiver_id);
         if (it == users.end()) {
         std::cout << "RouteMessage has not found receiver " << receiver_id << ".\n";
            response->set_success(false);
         } else {
            // Add the message to the receiver's queue
            messages_[receiver_id].push_back(*request);
            response->set_success(true);
         }

         return Status::OK;
      }

      Status GetMessages(ServerContext* context, const GetMessagesRequest* request,
                        GetMessagesResponse* response) override {
         std::lock_guard<std::mutex> lock(mu_);

         int id = request->id();
         std::cout << "GetMessages received message from " << id << ".\n";
         auto it = messages_.find(id);
         if (it != messages_.end()) {
            for (const auto& message : it->second) {
               RouteRequest* message_copy = response->add_messages();
               *message_copy = message;
            }
            // Clear the message queue for this client
            it->second.clear();
         }

         return Status::OK;
      }

   private:
      std::mutex mu_;
      // std::map<int, std::string> directory_;       ChatGPT mixed with users
      std::map<int, int> next_id_;
      std::map<int, std::vector<RouteRequest>> messages_;
   
      // Store user data in a map
      std::map<int, std::string> users;

};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  ChatServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}