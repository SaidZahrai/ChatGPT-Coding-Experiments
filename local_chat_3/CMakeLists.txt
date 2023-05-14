cmake_minimum_required(VERSION 3.10)

project(chat_app)

set(CMAKE_CXX_STANDARD 17)

# Find required protobuf package
find_package(Protobuf REQUIRED)

# Find required gRPC package
find_package(gRPC REQUIRED)

# Protobuf and gRPC paths
set(PROTOS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/protos")
set(PROTOS ${PROTOS_PATH}/chat.proto)
set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})

include_directories(${PROTO_SRC_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Added later in the process - with a correction ijn the path
set(GRPC_CPP_PLUGIN "/bin/grpc_cpp_plugin")

# Generate protobuf and gRPC files
add_custom_command(
    OUTPUT "${PROTO_SRC_DIR}/chat.pb.cc"
           "${PROTO_SRC_DIR}/chat.pb.h"
           "${PROTO_SRC_DIR}/chat.grpc.pb.cc"
           "${PROTO_SRC_DIR}/chat.grpc.pb.h"
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --grpc_out "${PROTO_SRC_DIR}"
         --cpp_out "${PROTO_SRC_DIR}"
         -I "${PROTOS_PATH}"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
         "${PROTOS}"
    DEPENDS "${PROTOS}"
)

# Add the executables
add_executable(chat_server src/server.cpp "${PROTO_SRC_DIR}/chat.pb.cc" "${PROTO_SRC_DIR}/chat.grpc.pb.cc")
target_link_libraries(chat_server gRPC::grpc++ gRPC::grpc++_reflection protobuf::libprotobuf)

add_executable(chat_client src/client.cpp "${PROTO_SRC_DIR}/chat.pb.cc" "${PROTO_SRC_DIR}/chat.grpc.pb.cc")
target_link_libraries(chat_client gRPC::grpc++_reflection gRPC::grpc++ protobuf::libprotobuf)
