add_subdirectory(abstractclient)
add_subdirectory(simple)
add_subdirectory(timeouts)
add_subdirectory(types)

add_custom_target(examples_as_tests
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/util/examples_as_tests.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})