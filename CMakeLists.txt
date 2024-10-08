cmake_minimum_required(VERSION 3.18)
project(
  uinta
  VERSION 0.1
  LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if("Debug" STREQUAL "${CMAKE_BUILD_TYPE}")
  add_definitions(-DUINTA_DEBUG)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(UINTA_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(assimp)
include(glm)
include(imgui)
include(stb)

set(UINTA_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
list(APPEND UINTA_INCLUDES "${UINTA_GENERATED_DIR}")

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -Wpedantic")

add_subdirectory(src/app)
add_subdirectory(src/engine)
add_subdirectory(src/platform)

configure_file(cmake/cfg.h.in ${UINTA_GENERATED_DIR}/uinta/cfg.h)
message(STATUS "Created configuration file: ${UINTA_GENERATED_DIR}/uinta/cfg.h")
