project(model)
add_executable(model main.cpp)
target_link_libraries(model PRIVATE uinta)
target_include_directories(model PRIVATE ${GL_INCLUDES})

