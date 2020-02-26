function(gen_inja_h)
foreach(source IN LISTS ARGV)
add_custom_command(
  OUTPUT ${source}.h
  COMMAND xxd -i ${source} > ${source}.h
  COMMAND touch -r ${source} ${source}.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${source}
)
endforeach()
endfunction()
