# This script should work as long as GLAD was built by running /path/to/engine/configure

find_path(GLAD_INCLUDE names "glad/glad.h" PATHS "${CMAKE_SOURCE_DIR}/dep/glad/build/include")
find_file(GLAD_C names "glad.c" PATHS ${CMAKE_SOURCE_DIR}/dep/glad/build/src REQUIRED)

if (NOT GLAD_C)
    message(FATAL_ERROR "Failed to locate GLAD. Possibly haven't run the configuration script?")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLAD DEFAULT_MSG GLAD_C GLAD_INCLUDE)
