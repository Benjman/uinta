cmake_minimum_required(VERSION 3.18)
project(engine)

set(CMAKE_CXX_STANDARD 23)

list(APPEND ENGINE_SRCS src/camera.cc)
list(APPEND ENGINE_SRCS src/color.cc)
list(APPEND ENGINE_SRCS src/debug/debug_scene.cc)
list(APPEND ENGINE_SRCS src/engine.cc)
list(APPEND ENGINE_SRCS src/file.cc)
list(APPEND ENGINE_SRCS src/font.cc)
list(APPEND ENGINE_SRCS src/math/aabb.cc)
list(APPEND ENGINE_SRCS src/mesh.cc)
list(APPEND ENGINE_SRCS src/scene.cc)
list(APPEND ENGINE_SRCS src/scenes/axis_viewer.cc)
list(APPEND ENGINE_SRCS src/scenes/grid.cc)
list(APPEND ENGINE_SRCS src/shader.cc)
list(APPEND ENGINE_SRCS src/texture.cc)
list(APPEND ENGINE_SRCS src/uniform.cc)
list(APPEND ENGINE_SRCS src/utils/fbx.cc)
list(APPEND ENGINE_SRCS src/utils/frame_manager.cc)
list(APPEND ENGINE_SRCS src/vbo.cc)

add_library(engine STATIC ${ENGINE_SRCS})
add_library(uinta::engine ALIAS engine)
target_include_directories(
  engine
  PUBLIC include
  PRIVATE ${UINTA_INCLUDES})
list(APPEND ENGINE_LIBS uinta::platform)
target_link_libraries(engine PRIVATE ${ENGINE_LIBS} ${UINTA_LIBS})

list(APPEND UINTA_RES_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(UINTA_RES_PATHS
    ${UINTA_RES_PATHS}
    PARENT_SCOPE)

find_package(GTest)
if(GTest_FOUND)
  add_subdirectory(test)
endif()
