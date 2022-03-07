cmake_minimum_required(VERSION 3.18)
project(app)

find_package(GLFW REQUIRED)

add_executable(app ./main.cpp
    ${GLAD_C})

target_link_libraries(app PRIVATE uinta ${GL_LIBS})
target_include_directories(app PRIVATE ${GL_INCLUDES})
