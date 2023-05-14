cmake_minimum_required(VERSION 3.13)

project(ChatApp)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required protobuf package
# find_package(protobuf CONFIG REQUIRED) Error from ChatGPT
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

# Find required gRPC package
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

# Include generated *.pb.h files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Proto file
get_filename_component(chat_proto "protos/chat.proto" ABSOLUTE)
get_filename_component(chat_proto_path "${chat_proto}" PATH)

# Generated sources
set(chat_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.cc")
set(chat_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.h")
set(chat_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc")
set(chat_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.h")

# Add custom command to generate protocol buffers
add_custom_command(
      OUTPUT "${chat_proto_srcs}" "${chat_proto_hdrs}" "${chat_grpc_srcs}" "${chat_grpc_hdrs}"
      COMMAND protobuf::protoc
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${chat_proto_path}"
        # --plugin=protoc-gen-grpc="${_gRPC_PROTOC_PLUGIN}"
        --plugin=protoc-gen-grpc="/bin/grpc_cpp_plugin"
        "${chat_proto}"
      DEPENDS "${chat_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Targets server.cpp and client.cpp
foreach(_target
  server client)
    add_executable(${_target} "src/${_target}.cpp"
      ${chat_proto_srcs}
      ${chat_grpc_srcs})
    target_link_libraries(${_target}
      protobuf::libprotobuf
      gRPC::grpc++
      gRPC::grpc++_reflection)
endforeach()
