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

set(UINTA_GENERATED_PATH  ${CMAKE_BINARY_DIR}/generated)
set(UINTA_INCLUDES        ${UINTA_INCLUDES} ${UINTA_GENERATED_PATH})
set(UINTA_LIBS_DIR        ${CMAKE_SOURCE_DIR}/lib)
set(UINTA_RES_PATHS       ${CMAKE_CURRENT_LIST_DIR}/res)

set(UINTA_SRCS ${UINTA_SRCS}

  ./src/camera/target_camera.cpp
  ./src/file_manager.cpp
  ./src/font.cpp
  ./src/gl/fbo.cpp
  ./src/gl/type_utils.cpp
  ./src/gl/vao.cpp
  ./src/gl/vbo.cpp
  ./src/gl/vertex_attrib.cpp
  ./src/logging.cpp
  ./src/math/running_avg.cpp
  ./src/memory/memory_link.cpp
  ./src/mesh.cpp
  ./src/metrics.cpp
  ./src/model_manager.cpp
  ./src/parsers/obj.cpp
  ./src/quadtree.cpp
  ./src/runner.cpp
  ./src/runner/args.cpp
  ./src/runner/display.cpp
  ./src/runner/scene.cpp
  ./src/shader.cpp
  ./src/utils/cartesian_grid.cpp
  ./src/utils/direction.cpp

  )

# dependencies
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${UINTA_LIBS_DIR}")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${UINTA_LIBS_DIR}/glad/include")
set(UINTA_SRCS     ${UINTA_SRCS}     "${UINTA_LIBS_DIR}/glad/src/glad.c")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${UINTA_LIBS_DIR}/glm")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${UINTA_LIBS_DIR}/imgui")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${UINTA_LIBS_DIR}/implot")

add_subdirectory(${UINTA_LIBS_DIR}/entt)
set(UINTA_LIBS ${UINTA_LIBS} EnTT::EnTT)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(${UINTA_LIBS_DIR}/glfw)
set(UINTA_LIBS ${UINTA_LIBS} glfw)

set(SPDLOG_BUILD_SHARED TRUE)
add_subdirectory(${UINTA_LIBS_DIR}/spdlog)
set(UINTA_LIBS ${UINTA_LIBS} spdlog)

add_library(uinta STATIC ${UINTA_SRCS})
include_directories(./include ${UINTA_INCLUDES})
target_link_libraries(uinta PUBLIC ${UINTA_LIBS})

add_subdirectory(app)

# testing disabled unless cmake was run with an enabling flag: `cmake -DUINTA_TEST=true`
if (UINTA_TEST)
  message(STATUS "Including tests")
  set(INSTALL_GTEST ON CACHE BOOL "Createe googletest binaries.")
  add_subdirectory(${UINTA_LIBS_DIR}/googletest)
  add_subdirectory(test)
endif()

message(STATUS "Resource paths found: ${UINTA_RES_PATHS}")

configure_file(./cmake/cfg.hpp.in ${UINTA_GENERATED_PATH}/uinta/cfg.hpp)
message(STATUS "Created configuration file: ${UINTA_GENERATED_PATH}/uinta/cfg.hpp")
