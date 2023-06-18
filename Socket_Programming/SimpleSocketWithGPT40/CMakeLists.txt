cmake_minimum_required(VERSION 3.10)

# set the project name
project(SocketProject)

# add the executable
add_executable(server server.cpp)
add_executable(client client.cpp)

# link pthread
find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads)
