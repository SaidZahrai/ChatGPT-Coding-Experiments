find_package(Threads REQUIRED)

find_package(Protobuf CONFIG REQUIRED)                       # Not included by ChatGPT
message(STATUS "Using protobuf ${Protobuf_VERSION}")         # Not included by ChatGPT

find_package(gRPC CONFIG REQUIRED)                           # Not included by ChatGPT
message(STATUS "Using gRPC ${gRPC_VERSION}")                 # Not included by ChatGPT

set(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/chat.proto")     # Not included by ChatGPT
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated") # Not included by ChatGPT

add_library(chat_system
  chat_client.cpp
  chat_server.cpp
)

target_include_directories(chat_system
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${GENERATED_PROTOBUF_PATH}                               # Not included by ChatGPT
)

target_link_libraries(chat_system
  PRIVATE 
    proto
    gRPC::grpc++
)

add_executable(client client_main.cpp)
target_link_libraries(client chat_system)

add_executable(server server_main.cpp)
target_link_libraries(server chat_system)
