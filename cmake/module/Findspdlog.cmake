find_path(SPDLOG_INCLUDE names "spdlog/spdlog.h" PATHS "${CMAKE_SOURCE_DIR}/lib/spdlog/include")
find_path(SPDLOG_SRC names "spdlog.cpp" PATHS "${CMAKE_SOURCE_DIR}/lib/spdlog/src")

set(UINTA_INCLUDES ${UINTA_INCLUDES} ${SPDLOG_INCLUDE} PARENT_SCOPE)
set(UINTA_SRCS ${UINTA_SRCS} ${SPDLOG_SRC} PARENT_SCOPE)

if (NOT SPDLOG_INCLUDE)
    message(FATAL_ERROR "could not locate header file in ${CMAKE_SOURCE_DIR}/lib")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stb DEFAULT_MSG SPDLOG_INCLUDE)
