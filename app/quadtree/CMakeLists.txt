project(quadtree)

add_executable(quadtree main.cpp ./quadtreeRunner.cpp)

target_link_libraries(quadtree PRIVATE uinta)
target_include_directories(quadtree PRIVATE ${GL_INCLUDES})

