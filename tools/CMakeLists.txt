include(flatbuf)

build_flatbuf(reflection.fbs)

add_executable(flatrpc
  flatrpc.cpp
  flatrpc_inja/flatrpc.hpp.inja.h
  flatrpc_inja/flatrpc.cpp.inja.h
)
set_property(TARGET flatrpc PROPERTY CXX_STANDARD 20)

target_link_libraries(flatrpc docopt inja)

function(gen_inja_h)
foreach(source IN LISTS ARGV)
add_custom_command(
  OUTPUT ${source}.h
  COMMAND xxd -i ${source} > ${source}.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${source}
)
endforeach()
endfunction()

gen_inja_h(
  flatrpc_inja/flatrpc.hpp.inja
  flatrpc_inja/flatrpc.cpp.inja
)
