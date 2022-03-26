find_path(STB_INCLUDE names "stb_truetype.h" PATHS "${CMAKE_SOURCE_DIR}/lib/stb")

if (NOT STB_INCLUDE)
    message(FATAL_ERROR "could not locate stb_truetype.h")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stb DEFAULT_MSG STB_INCLUDE)
