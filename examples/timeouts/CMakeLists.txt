project(flatrpc-example-timeout)
cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
include(flatbuf)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
add_subdirectory(../../rpc rpc)
add_subdirectory(../../vendor vendor)
add_subdirectory(../../compiler compiler)
endif()

build_flatbuf(timeout.fbs)

add_custom_command(OUTPUT timeout_flatrpc.hpp timeout_flatrpc.cpp
  COMMAND ${CMAKE_BINARY_DIR}/compiler/flatrpcc ${CMAKE_CURRENT_SOURCE_DIR}/timeout.fbs
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/timeout.fbs flatrpcc
  )

add_executable(timeouts_server server.cpp expensive_server.cpp timeout_flatrpc.cpp timeout_flatrpc.hpp)
set_property(TARGET timeouts_server PROPERTY CXX_STANDARD 20)
target_link_libraries(timeouts_server frozen flatrpc flatbuffers)
target_include_directories(timeouts_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(timeouts_client client.cpp timeout_flatrpc.cpp timeout_flatrpc.hpp)
set_property(TARGET timeouts_client PROPERTY CXX_STANDARD 20)
target_link_libraries(timeouts_client frozen flatrpc flatbuffers)
target_include_directories(timeouts_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
