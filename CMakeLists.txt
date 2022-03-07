cmake_minimum_required(VERSION 3.18)
project(uinta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(THREADS_PREFER_PTHREAD_FLAG ON)

add_library(uinta STATIC 
    ./src/mesh.cpp
    ./src/quadtree.cpp)

include_directories(./include)

# dependencies
find_package(GTest REQUIRED)
add_subdirectory(./lib/entt)

target_link_libraries(uinta PUBLIC EnTT::EnTT)

add_subdirectory(app)
add_subdirectory(test)
