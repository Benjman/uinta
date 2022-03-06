cmake_minimum_required(VERSION 3.18)
project(uinta)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(uinta STATIC ./src/quadtree.cpp)

# dependencies
find_package(GTest REQUIRED)
add_subdirectory(./lib/entt)

target_link_libraries(uinta PUBLIC EnTT::EnTT)

include_directories(./include)

add_subdirectory(test)
