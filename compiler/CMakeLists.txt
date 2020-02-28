include(flatbuf)
include(inja)

include(GNUInstallDirs)

build_flatbuf(reflection.fbs)

gen_inja_h(
  flatrpcc_inja/flatrpc.hpp.inja
  flatrpcc_inja/flatrpc.cpp.inja
)

add_executable(flatrpcc
  flatrpcc.cpp
  flatrpcc_inja/flatrpc.hpp.inja.h
  flatrpcc_inja/flatrpc.cpp.inja.h
)

set_property(TARGET flatrpcc PROPERTY CXX_STANDARD 20)

target_link_libraries(flatrpcc docopt inja)

target_include_directories(flatrpcc PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

install(
  TARGETS flatrpcc
  RUNTIME DESTINATION
  ${CMAKE_INSTALL_BINDIR}
)
