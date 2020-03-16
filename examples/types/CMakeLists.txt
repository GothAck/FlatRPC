project(flatrpc-example-types)
cmake_minimum_required(VERSION 3.13)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
include(flatbuf)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
add_subdirectory(../../rpc rpc)
add_subdirectory(../../vendor vendor)
add_subdirectory(../../compiler compiler)
endif()

build_flatbuf(types.fbs)

add_custom_command(OUTPUT types_flatrpc.hpp types_flatrpc.cpp
  COMMAND ${CMAKE_BINARY_DIR}/compiler/flatrpcc ${CMAKE_CURRENT_SOURCE_DIR}/types.fbs
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/types.fbs flatrpcc
  )

add_executable(types_server server.cpp types_server.cpp types_flatrpc.cpp types_flatrpc.hpp)
set_property(TARGET types_server PROPERTY CXX_STANDARD 20)
target_link_libraries(types_server frozen flatrpc flatbuffers)
target_include_directories(types_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(types_client client.cpp types_flatrpc.cpp types_flatrpc.hpp)
set_property(TARGET types_client PROPERTY CXX_STANDARD 20)
target_link_libraries(types_client frozen flatrpc flatbuffers)
target_include_directories(types_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
