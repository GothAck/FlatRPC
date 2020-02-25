set(WITH_TESTS OFF CACHE BOOL "disable eternal WITH_TESTS" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "disable inja BUILD_TESTING" FORCE)
set(BUILD_BENCHMARK OFF CACHE BOOL "disable inja BUILD_BENCHMARK" FORCE)

add_subdirectory(eternal)
add_subdirectory(frozen)
add_subdirectory(inja)
add_subdirectory(plog)
