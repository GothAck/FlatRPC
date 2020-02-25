include(flatbuf)

# TODO: COMPLETE ME


function(build_flatrpc)
  set(flatbuf_outputs)
  set(isinternal)
  list(GET ARGN 0 isinternal)
  if (isinternal EQUAL "INTERNAL")
    list(REMOVE_AT ARGN 0)
    set(isinternal TRUE)
  else()
    set(isinternal FALSE)
  endif()

  if (isinternal)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../../build/tools/flatrpc
        COMMAND mkdir -p build && cd build && cmake .. && make flatrpc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../..
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../..)
  endif()

  foreach (input IN LISTS ARGN)
    add_custom_command(OUTPUT simple_flatrpc.hpp
      COMMAND "$<IF:isinternal:${CMAKE_CURRENT_SOURCE_DIR}/../../build/tools/:>flatrpc" simple.fbs
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/simple.fbs $<IF:isinternal:${CMAKE_CURRENT_SOURCE_DIR}/../../build/tools/flatrpc:>
      )
  endforeach()
endfunction()
