find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/CMakeLists.txt")
  add_subdirectory(lib/glad)
  list(APPEND PLATFORM_LIBS glad)
endif()
list(APPEND PLATFORM_LIBS glfw ${OPENGL_LIBRARIES} ${GLU_LIBRARIES})