project(font)
add_executable(font main.cpp)
target_link_libraries(font PRIVATE uinta)
target_include_directories(font PRIVATE ${GL_INCLUDES})
