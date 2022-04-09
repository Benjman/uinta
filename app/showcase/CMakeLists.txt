project(showcase)
add_executable(showcase main.cpp runner.cpp ./src/debug.cpp)
target_link_libraries(showcase PRIVATE uinta)
target_include_directories(showcase PRIVATE ${GL_INCLUDES})
