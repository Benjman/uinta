cmake_minimum_required(VERSION 3.18)
project(uinta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/module")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# dependencies
find_package(GLFW REQUIRED)
find_package(GTest REQUIRED)
find_package(stb REQUIRED)
add_subdirectory(./lib/entt)

set(RES_PATH ${CMAKE_BINARY_DIR}/res/)
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/cmake/cfg.hpp.in ${CMAKE_BINARY_DIR}/generated/cfg.hpp)

add_library(uinta STATIC 

  ./src/camera.cpp
  ./src/file.cpp
  ./src/font.cpp
  ./src/glfw.cpp
  ./src/math.cpp
  ./src/mesh.cpp
  ./src/metrics.cpp
  ./src/quadtree.cpp
  ./src/shader.cpp

  ./app/utils/debug_controller.cpp # TODO not a great place to add this source

  ${GLAD_C})

include_directories(${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/generated
    ${GL_INCLUDES}
    ${STB_INCLUDE})

target_link_libraries(uinta PUBLIC EnTT::EnTT ${GL_LIBS})

add_subdirectory(app)
add_subdirectory(test)
