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
set(UINTA_LIB_PATH        ${CMAKE_BINARY_DIR}/lib)
set(UINTA_RES_PATHS       ${CMAKE_CURRENT_LIST_DIR}/res)

set(UINTA_SRCS ${UINTA_SRCS}

  ./src/camera/target_camera.cpp
  ./src/file_manager.cpp
  ./src/font.cpp
  ./src/gl.cpp
  ./src/math.cpp
  ./src/memory/memory_link.cpp
  ./src/mesh.cpp
  ./src/metrics.cpp
  ./src/model.cpp
  ./src/quadtree.cpp
  ./src/runner.cpp
  ./src/runner/display.cpp
  ./src/shader.cpp
  ./src/utils/cartesian_grid.cpp
  ./src/utils/running_avg.cpp

  )

# dependencies
set(UINTA_SRCS     ${UINTA_SRCS}     "${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${CMAKE_SOURCE_DIR}/lib/glad/include")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${CMAKE_SOURCE_DIR}/lib/glm")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${CMAKE_SOURCE_DIR}/lib/imgui")
set(UINTA_INCLUDES ${UINTA_INCLUDES} "${CMAKE_SOURCE_DIR}/lib") # needed for stb library FIXME

add_subdirectory(./lib/entt)
set(UINTA_LIBS ${UINTA_LIBS} EnTT::EnTT)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(./lib/glfw)
set(UINTA_LIBS ${UINTA_LIBS} glfw)

set(SPDLOG_BUILD_SHARED TRUE)
add_subdirectory(./lib/spdlog)
set(UINTA_LIBS ${UINTA_LIBS} spdlog)

add_library(uinta STATIC ${UINTA_SRCS})
include_directories(./include ${UINTA_INCLUDES})
target_link_libraries(uinta PUBLIC ${UINTA_LIBS})

set(UINTA_RES_PATHS ${CMAKE_CURRENT_LIST_DIR}/res)
add_subdirectory(app)

# testing disabled unless cmake was run with an enabling flag: `cmake -DUINTA_TEST=true`
if (UINTA_TEST)
  message(STATUS "Including tests")
  set(INSTALL_GTEST ON CACHE BOOL "Createe googletest binaries.")
  add_subdirectory(./lib/googletest)
  add_subdirectory(test)
endif()

message(STATUS "Resource paths found: ${UINTA_RES_PATHS}")

configure_file(./cmake/cfg.hpp.in ${UINTA_GENERATED_PATH}/uinta/cfg.hpp)
message(STATUS "Created configuration file: ${UINTA_GENERATED_PATH}/uinta/cfg.hpp")
