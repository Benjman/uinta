project(font)
add_executable(font main.cpp ./fontrunner.cpp)
target_link_libraries(font PRIVATE uinta)
target_include_directories(font PRIVATE ${GL_INCLUDES})
