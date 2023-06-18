cmake_minimum_required(VERSION 3.0)
project(SocketCommunication)

set(CMAKE_CXX_STANDARD 11)

# Server executable
add_executable(server server.cpp)
target_link_libraries(server PRIVATE pthread)

# Client executable
add_executable(client client.cpp)

