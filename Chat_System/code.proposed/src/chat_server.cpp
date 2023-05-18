#include "chat_server.h"

grpc::Status ChatServer::RegisterClient(grpc::ServerContext* context, const RegisterClientRequest* request,
                                        RegisterClientResponse* response) {
  for (const auto& client : clients) {
    if (client.second.name == request->name()) {
      response->set_id(-1);
      return grpc::Status::OK;
    }
  }

  int id = current_id++;
  clients[id] = {request->name(), {}};
  response->set_id(id);
  return grpc::Status::OK;
}

grpc::Status ChatServer::ListClients(grpc::ServerContext* context, const ListClientsRequest* request,
                                     ListClientsResponse* response) {
  for (const auto& client : clients) {
    Client* client_proto = response->add_clients();
    client_proto->set_id(client.first);
    client_proto->set_name(client.second.name);
  }

  return grpc::Status::OK;
}

grpc::Status ChatServer::SendMessage(grpc::ServerContext* context, const SendMessageRequest* request,
                                     SendMessageResponse* response) {
  if (clients.find(request->receiver_id()) == clients.end()) {
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Receiver not found");
  }

  Message message;
  message.set_sender_id(request->sender_id());
  message.set_receiver_id(request->receiver_id());
  message.set_message(request->message());

  clients[request->receiver_id()].messages.push(message);
  return grpc::Status::OK;
}

grpc::Status ChatServer::GetMessages(grpc::ServerContext* context, const GetMessagesRequest* request,
                                     GetMessagesResponse* response) {
  if (clients.find(request->client_id()) == clients.end()) {
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Client not found");
  }

  auto& messages = clients[request->client_id()].messages;
  while (!messages.empty()) {
    *response->add_messages() = messages.front();
    messages.pop();
  }

  return grpc::Status::OK;
}
