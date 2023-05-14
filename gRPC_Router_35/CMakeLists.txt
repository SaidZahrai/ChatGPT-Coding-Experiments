cmake_minimum_required(VERSION 3.12)
project(router_example)

set(CMAKE_CXX_STANDARD 17)

# Find required protobuf package
find_package(Protobuf REQUIRED)

# Find gRPC package
find_package(gRPC REQUIRED)

# Add generated gRPC code directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated)

# Add generated gRPC code to the server and client targets
add_executable(server server.cpp generated/router.grpc.pb.cc generated/router.pb.cc)
add_executable(client client.cpp generated/router.grpc.pb.cc generated/router.pb.cc)

# Link gRPC libraries
target_link_libraries(server gRPC::grpc++)
target_link_libraries(client gRPC::grpc++)

