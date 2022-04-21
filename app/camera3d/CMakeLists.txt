project(camera3d)
add_executable(camera3d main.cpp runner.cpp)
target_link_libraries(camera3d PRIVATE uinta)
target_include_directories(camera3d PRIVATE ${GL_INCLUDES})
