# This script should work as long as STB was built by running /path/to/engine/configure

find_path(STB_INCLUDE names "stb.h" PATHS "${CMAKE_SOURCE_DIR}/dep/stb")

if (NOT STB_INCLUDE)
    message(FATAL_ERROR "Failed to locate STB. Possibly haven't run the configuration script?")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(STB DEFAULT_MSG STB_INCLUDE)
