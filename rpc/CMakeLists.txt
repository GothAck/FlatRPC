add_subdirectory(fbs)

add_library(flatrpc
  STATIC
  rpcbase.cpp
  rpcclient.cpp
  rpcserver.cpp
  rpcsockets.cpp
  rpcrunnable.cpp
)

set_property(TARGET flatrpc PROPERTY CXX_STANDARD 20)

target_link_libraries(flatrpc plog rpc_fbs flatbuffers zmqpp zmq uuid pthread)

target_include_directories(flatrpc PUBLIC include PRIVATE include/rpc)
