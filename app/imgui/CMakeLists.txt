project(imgui)
add_executable(imgui main.cpp)
target_link_libraries(imgui PRIVATE uinta)
target_include_directories(imgui PRIVATE ${GL_INCLUDES})

