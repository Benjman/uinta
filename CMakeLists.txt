cmake_minimum_required(VERSION 3.18)
project(uinta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_BUILD_TYPE Debug)

set(THREADS_PREFER_PTHREAD_FLAG ON)

# dependencies
find_package(GLFW REQUIRED)
find_package(GTest REQUIRED)
add_subdirectory(./lib/entt)

add_library(uinta STATIC 
    ./src/camera2d.cpp
    ./src/math.cpp
    ./src/mesh.cpp
    ./src/quadtree.cpp
    ./src/runner.cpp
    ./src/window.cpp
    ${GLAD_C})

include_directories(./include)

target_link_libraries(uinta PUBLIC EnTT::EnTT ${GL_LIBS})
target_include_directories(uinta PRIVATE ${GL_INCLUDES})

add_subdirectory(app)
add_subdirectory(test)
