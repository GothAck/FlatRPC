project(flatrpc)
cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(vendor)
add_subdirectory(rpc)
add_subdirectory(compiler)
add_subdirectory(test)
add_subdirectory(examples)

export(TARGETS flatrpc plog rpc_fbs flatrpcc NAMESPACE FlatRPC:: FILE FlatRPCTargets.cmake)


set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE FlatRPC)
