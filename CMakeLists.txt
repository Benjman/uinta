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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(UINTA_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
list(APPEND UINTA_INCLUDES "${UINTA_GENERATED_DIR}")

set(UINTA_APP_CONFIG_PATH ${UINTA_GENERATED_DIR}/uinta/default.yml)
set(UINTA_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

if(UNIX)
  if(APPLE)
    set(UINTA_DATA_HOME "$ENV{HOME}/Library/Application Support/uinta")
  else()
    if(DEFINED ENV{XDG_RUNTIME_DIR})
      set(UINTA_DATA_DIR "$ENV{XDG_DATA_HOME}/uinta")
      set(UINTA_CACHE_DIR "$ENV{XDG_CACHE_HOME}/uinta")
      set(UINTA_RUNTIME_DIR "$ENV{XDG_RUNTIME_DIR}/uinta")
    else()
      set(UINTA_DATA_HOME "$ENV{HOME}/.local/share/uinta")
      set(UINTA_CACHE_DIR "$ENV{HOME}/.cache/uinta")
      set(UINTA_RUNTIME_DIR "${UINTA_DATA_HOME}")
    endif()
  endif()
elseif(WIN32)
  if(DEFINED ENV{APPDATA})
    set(UINTA_DATA_HOME "$ENV{APPDATA}/uinta")
  else()
    set(UINTA_DATA_HOME "$ENV{USERPROFILE}/AppData/Roaming/uinta")
    message(WARNING "APPDATA is not set. Using default path:
      ${UINTA_DATA_HOME}")
  endif()
else()
  set(UINTA_DATA_HOME "$ENV{HOME}/.local/share/uinta")
  message(WARNING "Unknown platform, using default path: ${UINTA_DATA_HOME}")
endif()

message(STATUS "Data directory: ${UINTA_DATA_DIR}")
message(STATUS "Cache directory: ${UINTA_CACHE_DIR}")
message(STATUS "Runtime directory: ${UINTA_RUNTIME_DIR}")

list(INSERT UINTA_RES_PATHS 0 "${UINTA_DATA_HOME}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(glm)
include(imgui)
include(stb)
include(yaml-cpp)

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -Wpedantic")

add_subdirectory(src/platform)
add_subdirectory(src/engine)
add_subdirectory(src/app)

configure_file(cmake/cfg.h.in ${UINTA_GENERATED_DIR}/uinta/cfg.h)
message(STATUS "Created configuration file: ${UINTA_GENERATED_DIR}/uinta/cfg.h")
