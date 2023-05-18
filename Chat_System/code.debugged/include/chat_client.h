#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

class ChatClient {
 private:
  std::unique_ptr<ChatService::Stub> stub_;
  int id;
  std::string name_;      //Chat GPT4 forgot to add

 public:
  ChatClient(std::shared_ptr<grpc::Channel> channel, const std::string& name);

  std::vector<Client> ListClients();
  bool SendMessage(int receiver_id, const std::string& message);
  std::vector<Message> GetMessages();
  std::string GetName();  // ChatGPT4 forgot to add
};
