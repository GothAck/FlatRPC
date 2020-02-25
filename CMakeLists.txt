project(flatrpc)
cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(vendor)
add_subdirectory(rpc)
add_subdirectory(tools)
