cmake_minimum_required(VERSION 3.18)
project(uinta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DUINTA_DEBUG)
endif()

# dependencie
set(LIB_PATH ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/module")
find_package(GLFW REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(stb REQUIRED)
add_subdirectory(./lib/entt)

set(RES_PATH ${CMAKE_BINARY_DIR}/res/)
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/cmake/cfg.hpp.in ${CMAKE_BINARY_DIR}/generated/cfg.hpp)

add_library(uinta STATIC 

  ./src/camera.cpp
  ./src/file.cpp
  ./src/font.cpp
  ./src/input.cpp
  ./src/math.cpp
  ./src/mesh.cpp
  ./src/metrics.cpp
  ./src/model.cpp
  ./src/quadtree.cpp
  ./src/runner.cpp
  ./src/shader.cpp

  ${GLAD_C})

include_directories(${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/generated
    ${GLM_INCLUDE}
    ${GL_INCLUDES}
    ${IMGUI_INCLUDE}
    ${STB_INCLUDE}
    )

target_link_libraries(uinta PUBLIC EnTT::EnTT ${GL_LIBS})

add_subdirectory(app)

# to enable testing run `cmake -DUINTA_TEST=true ...`
if (UINTA_TEST)
  add_subdirectory(test)
endif()
