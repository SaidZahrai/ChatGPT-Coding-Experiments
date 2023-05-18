find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

set(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/chat.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PROTOBUF_PATH})

set(PROTO_SRC "${GENERATED_PROTOBUF_PATH}/chat.pb.cc")
set(PROTO_HEADER "${GENERATED_PROTOBUF_PATH}/chat.pb.h")
set(GRPC_SRC "${GENERATED_PROTOBUF_PATH}/chat.grpc.pb.cc")
set(GRPC_HEADER "${GENERATED_PROTOBUF_PATH}/chat.grpc.pb.h")

add_custom_command(
  OUTPUT "${PROTO_SRC}" "${PROTO_HEADER}" "${GRPC_SRC}" "${GRPC_HEADER}"
  COMMAND ${Protobuf_PROTOC_EXECUTABLE}
  ARGS --grpc_out "${GENERATED_PROTOBUF_PATH}"
       --cpp_out "${GENERATED_PROTOBUF_PATH}"
       -I "${CMAKE_CURRENT_SOURCE_DIR}"
       --plugin=protoc-gen-grpc="${gRPC_CPP_PLUGIN_EXECUTABLE}"
       "${PROTO_PATH}"
  )

add_library(proto ${PROTO_SRC} ${GRPC_SRC})
target_include_directories(proto PUBLIC ${GENERATED_PROTOBUF_PATH})
target_link_libraries(proto ${Protobuf_LIBRARIES} gRPC::grpc++)
