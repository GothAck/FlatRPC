include(flatbuf)

build_flatbuf(rpc.fbs)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rpc_generated.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/flatrpc)
