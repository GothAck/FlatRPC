project(rpc)

add_subdirectory(fbs)

add_library(rpc
  STATIC
  rpcbase.cpp
  rpcclient.cpp
  rpcserver.cpp
  rpcsockets.cpp
  rpcrunnable.cpp
)

set_property(TARGET rpc PROPERTY CXX_STANDARD 20)

target_link_libraries(rpc plog rpc_fbs flatbuffers zmqpp zmq uuid pthread)

target_include_directories(rpc PUBLIC include PRIVATE include/rpc)
