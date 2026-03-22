cmake_minimum_required(VERSION 3.18)
project(engine)

set(CMAKE_CXX_STANDARD 20)

list(APPEND ENGINE_SRCS src/app_config_yaml.cc)
list(APPEND ENGINE_SRCS src/camera/camera.cc)
list(APPEND ENGINE_SRCS src/camera/camera_manager.cc)
list(APPEND ENGINE_SRCS src/color.cc)
list(APPEND ENGINE_SRCS src/debug/debug_scene.cc)
list(APPEND ENGINE_SRCS src/debug/debug_scene_ui.cc)
list(APPEND ENGINE_SRCS src/engine.cc)
list(APPEND ENGINE_SRCS src/font.cc)
list(APPEND ENGINE_SRCS src/input/input_system.cc)
list(APPEND ENGINE_SRCS src/lib/stb_image_api.cc)
list(APPEND ENGINE_SRCS src/localization/localization.cc)
list(APPEND ENGINE_SRCS src/localization/localization_system.cc)
list(APPEND ENGINE_SRCS src/manifold_mesh.cc)
list(APPEND ENGINE_SRCS src/math/aabb.cc)
list(APPEND ENGINE_SRCS src/mesh.cc)
list(APPEND ENGINE_SRCS src/reflect.cc)
list(APPEND ENGINE_SRCS src/scene.cc)
list(APPEND ENGINE_SRCS src/scene_system.cc)
list(APPEND ENGINE_SRCS src/scenes/axis_viewer.cc)
list(APPEND ENGINE_SRCS src/scenes/grid.cc)
list(APPEND ENGINE_SRCS src/shader.cc)
list(APPEND ENGINE_SRCS src/shaders/basic_shader.cc)
list(APPEND ENGINE_SRCS src/texture.cc)
list(APPEND ENGINE_SRCS src/utils/fbx.cc)
list(APPEND ENGINE_SRCS src/vbo_guard.cc)
list(APPEND ENGINE_SRCS src/viewport/viewport_manager.cc)

add_library(engine STATIC ${ENGINE_SRCS})
add_library(uinta::engine ALIAS engine)
set_target_properties(engine PROPERTIES OUTPUT_NAME uinta_engine)
target_include_directories(
  engine
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE ${UINTA_INCLUDES})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
list(APPEND ENGINE_LIBS uinta::platform)
target_link_libraries(engine PUBLIC ${ENGINE_LIBS} ${UINTA_LIBS})

set(UINTA_ENGINE_RES "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(UINTA_ENGINE_RES ${UINTA_ENGINE_RES} PARENT_SCOPE)
list(APPEND UINTA_RES_PATHS "${UINTA_ENGINE_RES}")
set(UINTA_RES_PATHS ${UINTA_RES_PATHS} PARENT_SCOPE)

configure_file(${CMAKE_MODULE_PATH}/config.yml.in ${UINTA_APP_CONFIG_PATH})
message(STATUS "config.yml file: ${UINTA_APP_CONFIG_PATH}")

install(DIRECTORY res/ DESTINATION ${CMAKE_INSTALL_DATADIR}/uinta)

find_package(GTest)
if(GTest_FOUND)
  add_subdirectory(test)
endif()
