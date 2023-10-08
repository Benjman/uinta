cmake_minimum_required(VERSION 3.18)
project(uinta)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if ("Debug" STREQUAL "${CMAKE_BUILD_TYPE}")
  add_definitions(-DUINTA_DEBUG)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(UINTA_GENERATED_PATH  ${CMAKE_BINARY_DIR}/generated)
set(UINTA_INCLUDES        ${UINTA_INCLUDES} ${UINTA_GENERATED_PATH})
set(UINTA_LIBS_DIR        ${CMAKE_SOURCE_DIR}/lib)
set(UINTA_RES_PATHS       ${CMAKE_CURRENT_LIST_DIR}/res)

set(UINTA_SRCS

  ./src/args.cpp
  ./src/file_manager.cpp
  ./src/font.cpp
  ./src/gl/type_utils.cpp
  ./src/gl/vao.cpp
  ./src/gl/vbo.cpp
  ./src/gl/vertex_attrib.cpp
  ./src/grid.cpp
  ./src/logging.cpp
  ./src/math/hexagon.cpp
  ./src/math/running_avg.cpp
  ./src/mesh.cpp
  ./src/metrics.cpp
  ./src/model_manager.cpp
  ./src/parsers/obj.cpp
  ./src/runner.cpp
  ./src/scene.cpp
  ./src/shader.cpp
  ./src/target_camera.cpp
  ./src/utils/direction.cpp

  )

# dependencies
list(APPEND UINTA_INCLUDES "${UINTA_LIBS_DIR}")
list(APPEND UINTA_INCLUDES "${UINTA_LIBS_DIR}/glad/include")
list(APPEND UINTA_SRCS     "${UINTA_LIBS_DIR}/glad/src/glad.c")
list(APPEND UINTA_INCLUDES "${UINTA_LIBS_DIR}/glm")
list(APPEND UINTA_INCLUDES "${UINTA_LIBS_DIR}/imgui")

add_subdirectory(${UINTA_LIBS_DIR}/entt)
list(APPEND UINTA_LIBS EnTT::EnTT)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(${UINTA_LIBS_DIR}/glfw)
list(APPEND UINTA_LIBS glfw)

set(SPDLOG_BUILD_SHARED TRUE)
add_subdirectory(${UINTA_LIBS_DIR}/spdlog)
list(APPEND UINTA_LIBS spdlog)

add_library(uinta STATIC)
target_sources(uinta PRIVATE ${UINTA_SRCS})
target_include_directories(uinta PUBLIC ./include ${UINTA_INCLUDES})
target_link_libraries(uinta PUBLIC ${UINTA_LIBS})

add_subdirectory(app)

# Enable testing by running cmake with the flag: `cmake -DUINTA_TEST=true`
if (UINTA_TEST)
  message(STATUS "Including tests")
  set(INSTALL_GTEST ON CACHE BOOL "Create googletest binaries.")
  add_subdirectory(${UINTA_LIBS_DIR}/googletest)
  add_subdirectory(test)
endif()

message(STATUS "Resource paths found: ${UINTA_RES_PATHS}")

configure_file(./cmake/cfg.hpp.in ${UINTA_GENERATED_PATH}/uinta/cfg.hpp)
message(STATUS "Created configuration file: ${UINTA_GENERATED_PATH}/uinta/cfg.hpp")
