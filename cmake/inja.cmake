function(gen_inja_h)
foreach(source IN LISTS ARGV)

get_filename_component(sourcedir "${source}" DIRECTORY)

add_custom_command(
  OUTPUT ${source}.h
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${sourcedir}
  COMMAND xxd -i ${source} > ${CMAKE_CURRENT_BINARY_DIR}/${source}.h
  COMMAND touch -r ${source} ${CMAKE_CURRENT_BINARY_DIR}/${source}.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${source}
)
endforeach()
endfunction()
