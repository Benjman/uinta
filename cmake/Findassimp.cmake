# This script should work as long as assimp was built by running /path/to/engine/configure

find_path(ASSIMP_INCLUDE names "assimp/scene.h" PATHS "${CMAKE_SOURCE_DIR}/dep/assimp/include")
find_library(ASSIMP_LIB names assimp HINTS "${CMAKE_SOURCE_DIR}/dep/assimp/build/bin")

if (NOT ASSIMP_LIB)
    message(FATAL_ERROR "Failed to locate assimp. Possibly haven't run the configuration script?")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(assimp DEFAULT_MSG ASSIMP_LIB ASSIMP_INCLUDE)
