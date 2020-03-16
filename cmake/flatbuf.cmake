function(build_flatbuf)
    set(flatbuf_outputs)
    foreach (input IN LISTS ARGN)
        get_filename_component(flatbuf_name "${input}" NAME_WE)
        get_filename_component(flatbuf_dir "${input}" DIRECTORY)
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${flatbuf_name}_generated.h
            COMMAND
                flatc
                --cpp --gen-mutable --gen-name-strings --gen-object-api --gen-compare --reflect-names
                --cpp-std c++17
                ${input}
            DEPENDS ${input}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM
        )
        add_custom_target(
            gen_${flatbuf_name}_fbs
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${flatbuf_name}_generated.h
        )
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${flatbuf_name}_generated.h PROPERTIES GENERATED TRUE)
        add_library(${flatbuf_name}_fbs STATIC ${CMAKE_CURRENT_SOURCE_DIR}/${flatbuf_name}_generated.h)
        target_include_directories(${flatbuf_name}_fbs INTERFACE ./${flatbuf_dir})
        set_target_properties(${flatbuf_name}_fbs PROPERTIES LINKER_LANGUAGE CXX)
#        target_sources(${flatbuf_name}_fbs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${flatbuf_name}_generated.h)
    endforeach ()
endfunction()
