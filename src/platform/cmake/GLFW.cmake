find_package(glfw3 QUIET)

if(NOT glfw3_FOUND)
  option(GLFW_BUILD_DOCS OFF)
  option(GLFW_BUILD_EXAMPLES OFF)
  option(GLFW_BUILD_TESTS OFF)
  option(GLFW_INSTALL OFF)
  add_subdirectory(lib/glfw)
endif()

find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

add_subdirectory(lib/glad)
list(APPEND PLATFORM_LIBS glad)
list(APPEND PLATFORM_LIBS glfw ${OPENGL_LIBRARIES} ${GLU_LIBRARIES})
