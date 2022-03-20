project(camera_2d)

add_executable(camera_2d main.cpp ./camera_2drunner.cpp)

target_link_libraries(camera_2d PRIVATE uinta)
target_include_directories(camera_2d PRIVATE ${GL_INCLUDES})
