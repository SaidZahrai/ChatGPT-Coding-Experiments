#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

class ChatServer final : public ChatService::Service {
 private:
  struct ClientInfo {
    std::string name;
    std::queue<Message> messages;
  };

  std::map<int, ClientInfo> clients;
  int current_id = 1;

 public:
  grpc::Status RegisterClient(grpc::ServerContext* context, const RegisterClientRequest* request,
                              RegisterClientResponse* response) override;

  grpc::Status ListClients(grpc::ServerContext* context, const ListClientsRequest* request,
                           ListClientsResponse* response) override;

  grpc::Status SendMessage(grpc::ServerContext* context, const SendMessageRequest* request,
                           SendMessageResponse* response) override;

  grpc::Status GetMessages(grpc::ServerContext* context, const GetMessagesRequest* request,
                           GetMessagesResponse* response) override;
};
