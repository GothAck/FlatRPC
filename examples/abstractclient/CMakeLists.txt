project(flatrpc-example-abstractclient)
cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
include(flatbuf)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  add_subdirectory(../../rpc rpc)
  add_subdirectory(../../vendor vendor)
  add_subdirectory(../../compiler compiler)
endif()

build_flatbuf(abstractclient.fbs)

add_custom_command(OUTPUT abstractclient_flatrpc.hpp abstractclient_flatrpc.cpp
  COMMAND ${CMAKE_BINARY_DIR}/compiler/flatrpcc ${CMAKE_CURRENT_SOURCE_DIR}/abstractclient.fbs
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/abstractclient.fbs flatrpcc
  )

add_executable(abstractclient_server server.cpp greeter_server.cpp abstractclient_flatrpc.cpp abstractclient_flatrpc.hpp)
set_property(TARGET abstractclient_server PROPERTY CXX_STANDARD 20)
target_link_libraries(abstractclient_server frozen flatrpc flatbuffers)
target_include_directories(abstractclient_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(abstractclient_client client.cpp abstractclient_flatrpc.cpp abstractclient_flatrpc.hpp)
set_property(TARGET abstractclient_client PROPERTY CXX_STANDARD 20)
target_link_libraries(abstractclient_client frozen flatrpc flatbuffers)
target_include_directories(abstractclient_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
