cmake_minimum_required(VERSION 3.18)
project(
  uinta
  VERSION 0.1
  LANGUAGES CXX C)

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
set(UINTA_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

if(UNIX)
  if(APPLE)
    set(UINTA_DATA_DIR "$ENV{HOME}/Library/Application Support/uinta")
    set(UINTA_TMP_DIR "$ENV{TMPDIR}")
    if(NOT UINTA_TMP_DIR)
      set(UINTA_TMP_DIR "/tmp")
    endif()
  else()
    # Set data directory with XDG fallback
    if(DEFINED ENV{XDG_DATA_HOME} AND NOT "$ENV{XDG_DATA_HOME}" STREQUAL "")
      set(UINTA_DATA_DIR "$ENV{XDG_DATA_HOME}/uinta")
    else()
      set(UINTA_DATA_DIR "$ENV{HOME}/.local/share/uinta")
    endif()

    # Set cache directory with XDG fallback
    if(DEFINED ENV{XDG_CACHE_HOME} AND NOT "$ENV{XDG_CACHE_HOME}" STREQUAL "")
      set(UINTA_CACHE_DIR "$ENV{XDG_CACHE_HOME}/uinta")
    else()
      set(UINTA_CACHE_DIR "$ENV{HOME}/.cache/uinta")
    endif()

    # Set runtime directory with XDG fallback
    if(DEFINED ENV{XDG_RUNTIME_DIR} AND NOT "$ENV{XDG_RUNTIME_DIR}" STREQUAL "")
      set(UINTA_RUNTIME_DIR "$ENV{XDG_RUNTIME_DIR}/uinta")
      set(UINTA_TMP_DIR "$ENV{XDG_RUNTIME_DIR}")
    else()
      set(UINTA_RUNTIME_DIR "${UINTA_DATA_DIR}")
      set(UINTA_TMP_DIR "/tmp")
    endif()
  endif()
elseif(WIN32)
  if(DEFINED ENV{APPDATA})
    set(UINTA_DATA_DIR "$ENV{APPDATA}/uinta")
  else()
    set(UINTA_DATA_DIR "$ENV{USERPROFILE}/AppData/Roaming/uinta")
    message(WARNING "APPDATA is not set. Using default path:
      ${UINTA_DATA_DIR}")
  endif()
  if(DEFINED ENV{TEMP})
    set(UINTA_TMP_DIR "$ENV{TEMP}")
  elseif(DEFINED ENV{TMP})
    set(UINTA_TMP_DIR "$ENV{TMP}")
  else()
    set(UINTA_TMP_DIR "$ENV{USERPROFILE}/AppData/Local/Temp")
  endif()
else()
  set(UINTA_DATA_DIR "$ENV{HOME}/.local/share/uinta")
  set(UINTA_TMP_DIR "/tmp")
  message(WARNING "Unknown platform, using default path: ${UINTA_DATA_DIR}")
endif()

message(STATUS "Data directory: ${UINTA_DATA_DIR}")
message(STATUS "Cache directory: ${UINTA_CACHE_DIR}")
message(STATUS "Runtime directory: ${UINTA_RUNTIME_DIR}")

list(INSERT UINTA_RES_PATHS 0 "${UINTA_DATA_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(abseil-cpp)
include(glm)

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -Wpedantic")

enable_testing()

# Enable code coverage for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Code coverage enabled")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")

    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
    if(LCOV)
      set(LCOV_IGNORE_ERRORS mismatch,inconsistent,unused,source,range)
      add_custom_target(coverage
        COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete
        COMMAND ctest --output-on-failure
        COMMAND ${LCOV} --capture
          --directory ${CMAKE_BINARY_DIR}
          --output-file ${CMAKE_SOURCE_DIR}/lcov.info
          --rc branch_coverage=1
          --ignore-errors ${LCOV_IGNORE_ERRORS}
        COMMAND ${LCOV} --remove ${CMAKE_SOURCE_DIR}/lcov.info
          '*/lib/*'
          '*/test/*'
          '/usr/*'
          --output-file ${CMAKE_SOURCE_DIR}/lcov.info
          --rc branch_coverage=1
          --ignore-errors ${LCOV_IGNORE_ERRORS}
        COMMAND ${LCOV} --summary ${CMAKE_SOURCE_DIR}/lcov.info
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests and generating coverage report"
        VERBATIM
      )

      if(GENHTML)
        add_custom_target(coverage-html
          COMMAND ${GENHTML} ${CMAKE_SOURCE_DIR}/lcov.info
            --output-directory ${CMAKE_SOURCE_DIR}/coverage_html
            --title "Uinta Coverage Report"
            --legend
            --rc branch_coverage=1
            --ignore-errors inconsistent,source,corrupt
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
          COMMENT "Generating HTML coverage report"
          VERBATIM
        )
        add_dependencies(coverage-html coverage)
      endif()

      message(STATUS "Coverage targets available: 'coverage', 'coverage-html'")
    else()
      message(STATUS "lcov not found - coverage targets disabled")
    endif()
  endif()
endif()

add_subdirectory(src/platform)
add_subdirectory(src/engine)
add_subdirectory(src/app)

# Convert UINTA_RES_PATHS list to C++ array initializer
list(LENGTH UINTA_RES_PATHS UINTA_FILE_PATHS_COUNT)
set(UINTA_FILE_PATHS_INIT "")
foreach(path IN LISTS UINTA_RES_PATHS)
  if(UINTA_FILE_PATHS_INIT)
    string(APPEND UINTA_FILE_PATHS_INIT ",\n    ")
  endif()
  string(APPEND UINTA_FILE_PATHS_INIT "\"${path}\"")
endforeach()

configure_file(cmake/cfg.h.in ${UINTA_GENERATED_DIR}/uinta/cfg.h)
message(STATUS "Created configuration file: ${UINTA_GENERATED_DIR}/uinta/cfg.h")
