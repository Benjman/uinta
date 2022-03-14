cmake_minimum_required(VERSION 3.18)
project(app)

add_executable(app ./main.cpp
    ./runners/quadtreeRunner.cpp)

target_link_libraries(app PRIVATE uinta)

target_include_directories(app PRIVATE ${GL_INCLUDES})
