include(flatbuf)
include(inja)

include(GNUInstallDirs)

build_flatbuf(reflection.fbs)

gen_inja_h(
  flatrpc_inja/flatrpc.hpp.inja
  flatrpc_inja/flatrpc.cpp.inja
)

add_executable(flatrpc
  flatrpc.cpp
  flatrpc_inja/flatrpc.hpp.inja.h
  flatrpc_inja/flatrpc.cpp.inja.h
)
set_property(TARGET flatrpc PROPERTY CXX_STANDARD 20)

target_link_libraries(flatrpc docopt inja)

install(
  TARGETS flatrpc
  RUNTIME DESTINATION
  ${CMAKE_INSTALL_BINDIR}
)
