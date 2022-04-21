project(camera2d)
add_executable(camera2d main.cpp ./camera2drunner.cpp)
target_link_libraries(camera2d PRIVATE uinta)
target_include_directories(camera2d PRIVATE ${GL_INCLUDES})
