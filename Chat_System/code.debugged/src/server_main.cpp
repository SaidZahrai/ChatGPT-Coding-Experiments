#include <grpcpp/grpcpp.h>
#include "chat_server.h"

int main(int argc, char** argv) {
  std::string server_address("0.0.0.0:50051");
  ChatServer service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  std::cout << "Server listening on " << server_address << std::endl;
  std::cout << "Waiting for connection..." << std::endl;

  server->Wait();
  return 0;
}
