# This script should work as long as GLFW was built by running /path/to/engine/configure

find_path(GLFW_INCLUDE names "GLFW/glfw3.h" PATHS "${CMAKE_SOURCE_DIR}/dep/glfw/include")
find_library(GLFW_LIB names glfw3 HINTS "${CMAKE_SOURCE_DIR}/dep/glfw/build/src")

if (NOT GLFW_LIB)
    message(FATAL_ERROR "Failed to locate GLFW. Possibly haven't run the configuration script?")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIB GLFW_INCLUDE)
