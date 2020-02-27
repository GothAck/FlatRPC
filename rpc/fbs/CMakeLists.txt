include(flatbuf)

build_flatbuf(rpc.fbs)

install(FILES rpc_generated.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/flatrpc)
