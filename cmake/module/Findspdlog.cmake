find_path(SPDLOG_INCLUDE names "spdlog/spdlog.h" PATHS "${CMAKE_SOURCE_DIR}/lib/spdlog/include")
find_path(SPDLOG_SRC names "spdlog.cpp" PATHS "${CMAKE_SOURCE_DIR}/lib/spdlog/src")

if (NOT SPDLOG_INCLUDE)
    message(FATAL_ERROR "could not locate header file in ${CMAKE_SOURCE_DIR}/lib")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stb DEFAULT_MSG SPDLOG_INCLUDE)
