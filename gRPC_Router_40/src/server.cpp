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

class ChatServiceImpl final : public ChatService::Service {
   // Store user data in a map
   std::map<int, std::string> users;

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
                  RouteResponse* reply) override {
      // check if receiver exists in the directory
      if (users.find(request->receiver_id()) != users.end()) {
         // in a real world scenario, we would also send the message to the receiver
         // but for this simple example, we just acknowledge successful routing
         reply->set_success(true);
      } else {
         reply->set_success(false);
      }
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
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}