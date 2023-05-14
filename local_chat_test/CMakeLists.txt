cmake_minimum_required(VERSION 3.13)

# Name of the project
project(chat)

# Find protobuf and grpc
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_REFLECTION gRPC::grpc++_reflection)
message(STATUS "Using gRPC ${gRPC_VERSION}")

# Proto file
get_filename_component(chat_proto "protos/chat.proto" ABSOLUTE)
get_filename_component(chat_proto_path "${chat_proto}" PATH)

# Generated sources
set(chat_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.cc")
set(chat_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.h")
set(chat_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc")
set(chat_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.h")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


# Targets chat_server and chat_client
# foreach(_target
#   server
#   client)
#   add_executable(${_target} "src/${_target}.cpp"
#     ${chat_proto_srcs}
#     ${chat_grpc_srcs})
#   target_link_libraries(${_target}
#     ${_REFLECTION}
#     ${_GRPC_GRPCPP}
#     ${_PROTOBUF_LIBPROTOBUF})
# endforeach()

set(GRPC_CPP_PLUGIN "/bin/grpc_cpp_plugin")

message(STATUS "Building in ${CMAKE_CURRENT_BINARY_DIR} with ${Protobuf_PROTOC_EXECUTABLE}")
add_custom_command(
      OUTPUT "${chat_proto_srcs}" "${chat_proto_hdrs}" "${chat_grpc_srcs}" "${chat_grpc_hdrs}"
      COMMAND ${Protobuf_PROTOC_EXECUTABLE}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${chat_proto_path}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
        "${chat_proto}"
      DEPENDS "${chat_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Add the executables
add_executable(chat_server src/server.cpp "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.cc" "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc")
target_link_libraries(chat_server gRPC::grpc++ gRPC::grpc++_reflection protobuf::libprotobuf)

add_executable(chat_client src/client.cpp "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.cc" "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc")
target_link_libraries(chat_client gRPC::grpc++_reflection gRPC::grpc++ protobuf::libprotobuf)
