find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

add_subdirectory(lib/glad)
list(APPEND PLATFORM_LIBS glad)
list(APPEND PLATFORM_LIBS glfw ${OPENGL_LIBRARIES} ${GLU_LIBRARIES})