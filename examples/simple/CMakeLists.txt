project(flatrpc-example-simple)
cmake_minimum_required(VERSION 3.13)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
include(flatbuf)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
add_subdirectory(../../rpc rpc)
add_subdirectory(../../vendor vendor)
add_subdirectory(../../compiler compiler)
endif()

build_flatbuf(simple.fbs)

add_custom_command(OUTPUT simple_flatrpc.hpp simple_flatrpc.cpp
  COMMAND ${CMAKE_BINARY_DIR}/compiler/flatrpcc ${CMAKE_CURRENT_SOURCE_DIR}/simple.fbs
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/simple.fbs flatrpcc
  )

add_executable(simple_server server.cpp greeter_server.cpp simple_flatrpc.cpp simple_flatrpc.hpp)
set_property(TARGET simple_server PROPERTY CXX_STANDARD 20)
target_link_libraries(simple_server frozen flatrpc flatbuffers)
target_include_directories(simple_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(simple_client client.cpp simple_flatrpc.cpp simple_flatrpc.hpp)
set_property(TARGET simple_client PROPERTY CXX_STANDARD 20)
target_link_libraries(simple_client frozen flatrpc flatbuffers)
target_include_directories(simple_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
