project(flatrpc-example-types)
cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
include(flatbuf)

add_subdirectory(../../rpc rpc)
add_subdirectory(../../vendor vendor)
add_subdirectory(../../compiler compiler)

build_flatbuf(types.fbs)

add_custom_command(OUTPUT types_flatrpc.hpp types_flatrpc.cpp
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/compiler/flatrpcc ${CMAKE_CURRENT_SOURCE_DIR}/types.fbs
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/types.fbs flatrpcc
  )

add_executable(server server.cpp types_server.cpp types_flatrpc.cpp types_flatrpc.hpp)
set_property(TARGET server PROPERTY CXX_STANDARD 20)
target_link_libraries(server frozen flatrpc flatbuffers)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(client client.cpp types_flatrpc.cpp types_flatrpc.hpp)
set_property(TARGET client PROPERTY CXX_STANDARD 20)
target_link_libraries(client frozen flatrpc flatbuffers)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
