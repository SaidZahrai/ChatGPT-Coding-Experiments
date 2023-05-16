Test date May 16, 2023


```sh
chat_system_2 > ./compare_files
---------------------------------------------------
The file code.proposed/CMakeLists.txt was fine.
---------------------------------------------------
The file code.proposed/include/chat_client.h has the following corrections:
7a8
>   std::string name_;      //Chat GPT4 forgot to add
14a16
>   std::string GetName();  // ChatGPT4 forgot to add
---------------------------------------------------
The file code.proposed/include/chat_server.h has the following corrections:
3a4,5
> #include <queue>  // ChatGPT4 forgot it
>
---------------------------------------------------
The file code.proposed/proto/CMakeLists.txt has the following corrections:
19c19
<        --plugin=protoc-gen-grpc="${gRPC_CPP_PLUGIN_EXECUTABLE}"
---
>        --plugin=protoc-gen-grpc="/bin/grpc_cpp_plugin" ## ChatGPT's proposal did not work "${gRPC_CPP_PLUGIN_EXECUTABLE}"
---------------------------------------------------
The file code.proposed/proto/chat.proto was fine.
---------------------------------------------------
The file code.proposed/src/CMakeLists.txt has the following corrections:
2a3,11
> find_package(Protobuf CONFIG REQUIRED)                       # Not included by ChatGPT
> message(STATUS "Using protobuf ${Protobuf_VERSION}")         # Not included by ChatGPT
>
> find_package(gRPC CONFIG REQUIRED)                           # Not included by ChatGPT
> message(STATUS "Using gRPC ${gRPC_VERSION}")                 # Not included by ChatGPT
>
> set(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/chat.proto")     # Not included by ChatGPT
> set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated") # Not included by ChatGPT
>
10a20
>     ${GENERATED_PROTOBUF_PATH}                               # Not included by ChatGPT
---------------------------------------------------
The file code.proposed/src/chat_client.cpp has the following corrections:
63a64,67
>
> std::string ChatClient::GetName() { // ChatGPT4 forgot
>   return name_;
> }
---------------------------------------------------
The file code.proposed/src/chat_server.cpp was fine.
---------------------------------------------------
The file code.proposed/src/client_main.cpp has the following corrections:
8a9
>     std::mutex cout_mutex; // ChatGPT4 forgot!
10c11
<         std::this_thread::sleep_for(std::chrono::seconds(30));
---
>         std::this_thread::sleep_for(std::chrono::seconds(3));
22a24
>     std::mutex cout_mutex; // ChatGPT4 forgot!
33c35
<                 auto clients = client.GetClients();
---
>                 auto clients = client.ListClients(); // ChatGPT made mistate and called client.GetClients()
36c38
<                     std::cout << "Client name: " << client.first << ", Client ID: " << client.second << "\n";
---
>                     std::cout << "Client name: " << client.name() << ", Client ID: " << client.id() << "\n"; // GPT made mistake. Had to correct
---------------------------------------------------
The file code.proposed/src/server_main.cpp was fine.
```