cmake_minimum_required(VERSION 3.25)
project(echogrpcstream)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH C:/Library/grpc-v1.61.0)
set(Protobuf_USE_STATIC_LIBS ON)

find_package(utf8_range REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

set(PROTO_NAME Greet)

set(PROTO_SRC ${CMAKE_BINARY_DIR}/autogen/${PROTO_NAME}.pb.cc ${CMAKE_BINARY_DIR}/autogen/${PROTO_NAME}.pb.h
        ${CMAKE_BINARY_DIR}/autogen/${PROTO_NAME}.grpc.pb.cc ${CMAKE_BINARY_DIR}/autogen/${PROTO_NAME}.grpc.pb.h)

get_property(PLUGIN TARGET gRPC::grpc_cpp_plugin PROPERTY IMPORTED_LOCATION_RELEASE)

add_custom_command(
        OUTPUT ${PROTO_SRC}
        COMMAND protobuf::protoc
        ARGS --plugin=protoc-gen-grpc=${PLUGIN}
        --grpc_out=${CMAKE_BINARY_DIR}/autogen
        --cpp_out=${CMAKE_BINARY_DIR}/autogen
        -I ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/${PROTO_NAME}.proto)

add_executable(echoclient ${PROTO_SRC} client.cpp)
target_link_libraries(echoclient gRPC::grpc++ absl::log_internal_message absl::log_internal_check_op absl::log_internal_conditions utf8_range::utf8_validity)
target_include_directories(echoclient PRIVATE ${CMAKE_BINARY_DIR}/autogen)

add_executable(echoserver ${PROTO_SRC} server.cpp)
target_link_libraries(echoserver gRPC::grpc++ absl::log_internal_message absl::log_internal_check_op absl::log_internal_conditions utf8_range::utf8_validity)
target_include_directories(echoserver PRIVATE ${CMAKE_BINARY_DIR}/autogen)
