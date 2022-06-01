find_path(STB_INCLUDE names "stb/stb_truetype.h" PATHS "${CMAKE_SOURCE_DIR}/lib")
set(UINTA_INCLUDES ${UINTA_INCLUDES} ${STB_INCLUDE})

if (NOT STB_INCLUDE)
    message(FATAL_ERROR "could not locate stb_truetype.h in ${CMAKE_SOURCE_DIR}/lib")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stb DEFAULT_MSG STB_INCLUDE)
