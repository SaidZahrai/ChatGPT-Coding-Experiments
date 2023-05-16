find_package(Threads REQUIRED)

add_library(chat_system
  chat_client.cpp
  chat_server.cpp
)

target_include_directories(chat_system
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
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
