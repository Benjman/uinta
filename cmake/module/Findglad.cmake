find_path(GLAD_INCLUDE names "glad/glad.h" PATHS "${CMAKE_SOURCE_DIR}/lib/glad/include")
find_file(GLAD_C names "glad.c" PATHS "${CMAKE_SOURCE_DIR}/lib/glad/src" REQUIRED)

set(UINTA_INCLUDES ${UINTA_INCLUDES} ${GLAD_INCLUDE})
set(UINTA_SRCS ${UINTA_SRCS} ${GLAD_C})

if (NOT GLAD_C)
    message(FATAL_ERROR "Failed to locate GLAD. Possibly haven't run the configuration script?")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glad DEFAULT_MSG GLAD_C GLAD_INCLUDE)
