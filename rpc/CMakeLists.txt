include(GNUInstallDirs)

add_subdirectory(fbs)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc
  COMMAND echo '\#pragma once' > ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
  COMMAND echo -n '\#define FLATRPC_GITREV ' >> ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
  COMMAND git rev-parse --short HEAD | python3 -c 'import sys\; print (int(sys.stdin.read(), 16))' >> ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
  COMMAND echo -n '\#define FLATRPC_GITTAG ' >> ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
  COMMAND git describe --tags 2>/dev/null | head -n1 | cut -d- -f1 >> ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
)

add_library(flatrpc
  SHARED
  rpcbase.cpp
  rpcclient.cpp
  rpcserver.cpp
  rpcsockets.cpp
  rpcrunnable.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
)

add_library(flatrpc-static
  STATIC
  rpcbase.cpp
  rpcclient.cpp
  rpcserver.cpp
  rpcsockets.cpp
  rpcrunnable.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc/rpcversion.hpp
)

set_property(TARGET flatrpc PROPERTY CXX_STANDARD 20)

target_link_libraries(flatrpc plog rpc_fbs flatbuffers zmqpp zmq uuid pthread)

target_include_directories(flatrpc PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include PRIVATE include/flatrpc ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc)

set_property(TARGET flatrpc-static PROPERTY CXX_STANDARD 20)

target_link_libraries(flatrpc-static plog rpc_fbs flatbuffers zmqpp zmq uuid pthread)

target_include_directories(flatrpc-static PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include PRIVATE include/flatrpc ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc)

install(DIRECTORY include/flatrpc DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/flatrpc DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h*")

install(
  TARGETS flatrpc flatrpc-static
  LIBRARY DESTINATION
  ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION
  ${CMAKE_INSTALL_LIBDIR}
)
