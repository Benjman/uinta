cmake_minimum_required(VERSION 3.18)
project(
  uinta
  VERSION 0.0.20
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

set(UINTA_APP_CONFIG_PATH ${UINTA_GENERATED_DIR}/uinta/default.yml)
set(UINTA_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(abseil-cpp)
include(glm)
include(imgui)
include(stb)
include(yaml-cpp)

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -Wpedantic")

enable_testing()

# Code coverage instrumentation (--coverage) links libuinta_* against
# gcov, which is only resolvable by consumers that also build with
# --coverage. It's opt-in so installed Debug builds stay linkable by
# ordinary consumers; enable explicitly with -DUINTA_ENABLE_COVERAGE=ON.
option(UINTA_ENABLE_COVERAGE "Enable gcov/lcov code coverage instrumentation" OFF)

if(UINTA_ENABLE_COVERAGE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
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

set(UINTA_INSTALL_RES_DIR
  "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/uinta")
list(APPEND UINTA_RES_PATHS "${UINTA_INSTALL_RES_DIR}")

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

install(FILES ${UINTA_GENERATED_DIR}/uinta/cfg.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/uinta)

# glad and eventpp are vendored in-tree rather than found/fetched, so their
# public headers aren't installed by any cmake/*.cmake module.
install(DIRECTORY src/platform/lib/glad/include/ DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY src/platform/lib/eventpp/include/ DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR})

install(
  TARGETS platform engine glad eventpp imgui
  EXPORT uinta-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT uinta-targets
  NAMESPACE uinta::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uinta)

# Ship the find-then-fetch dependency modules so uinta-config.cmake can
# resolve the same third-party packages the same way, without duplicating
# that logic in consumer projects.
install(DIRECTORY cmake/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uinta/modules
  FILES_MATCHING PATTERN "*.cmake")
install(FILES src/platform/cmake/GLFW.cmake src/platform/cmake/eventpp.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uinta/modules)

configure_package_config_file(
  cmake/uinta-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/uinta-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uinta)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/uinta-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/uinta-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/uinta-config-version.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/uinta)
