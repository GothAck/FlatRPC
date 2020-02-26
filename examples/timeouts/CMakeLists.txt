project(flatrpc-example-timeout)
cmake_minimum_required(VERSION 3.13)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
include(flatbuf)

add_subdirectory(../../rpc rpc)
add_subdirectory(../../vendor vendor)
add_subdirectory(../../tools tools)

build_flatbuf(timeout.fbs)

add_custom_command(OUTPUT timeout_flatrpc.hpp timeout_flatrpc.cpp
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tools/flatrpc ${CMAKE_CURRENT_SOURCE_DIR}/timeout.fbs
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/timeout.fbs flatrpc
  )

add_executable(server server.cpp expensive_server.cpp timeout_flatrpc.cpp timeout_flatrpc.hpp)
set_property(TARGET server PROPERTY CXX_STANDARD 20)
target_link_libraries(server frozen rpc flatbuffers)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(client client.cpp timeout_flatrpc.cpp timeout_flatrpc.hpp)
set_property(TARGET client PROPERTY CXX_STANDARD 20)
target_link_libraries(client frozen rpc flatbuffers)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
