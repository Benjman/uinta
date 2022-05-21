project(ray_picking)
add_executable(ray_picking main.cpp)
target_link_libraries(ray_picking PRIVATE uinta)
target_include_directories(ray_picking PRIVATE ${GL_INCLUDES})
