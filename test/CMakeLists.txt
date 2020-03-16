add_executable(test_rpcrunnable
  test_rpcrunnable.cpp
)
set_property(TARGET test_rpcrunnable PROPERTY CXX_STANDARD 20)
target_link_libraries(test_rpcrunnable flatrpc)

add_custom_target(test
  COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -type f -executable -print0 | xargs -0 -n1 /usr/bin/env
  )

add_dependencies(test test_rpcrunnable)