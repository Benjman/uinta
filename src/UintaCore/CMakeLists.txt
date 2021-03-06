cmake_minimum_required(VERSION 3.15)

project(UintaCore
        VERSION 0.1)

set(SOURCES ${SOURCES}
		src/controller/scene/scene_controller.cpp
		src/controller/scene/scene_shader.cpp
		src/model/model.cpp
        src/camera/perspective_camera.cpp
        src/controller/buffer_controller.cpp
        src/controller/controller.cpp
		src/controller/scene/terrain.cpp
        src/debug/camera_ui_controller.cpp
        src/debug/cursor_controller.cpp
        src/debug/debug_controller.cpp
        src/debug/debug_ui_controller.cpp
        src/debug/debug_ui_shader.cpp
        src/debug/debug_world_controller.cpp
        src/debug/debug_world_shader.cpp
        src/debug/fps_controller.cpp
        src/debug/tick_controller.cpp
        src/gl/gl_error.cpp
        src/gl/gl_state.cpp
        src/input/input_manager_impl.cpp
        src/io/file.cpp
        src/render/i_render_controller.cpp
        src/render/i_renderable.cpp
        src/shader/shader.cpp
        src/shader/uniform.cpp
        src/shader/vao.cpp
        src/shader/vbo.cpp
        src/shader/vertex_attribute.cpp
        src/text/font.cpp
        src/text/text_controller.cpp
        src/texture/texture.cpp
        src/ui/ui.cpp
        src/ui/ui_element.cpp
        )

set(UINTA_CORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(UINTA_CORE_INCLUDE ${UINTA_CORE_INCLUDE} PARENT_SCOPE)

set(INCLUDES ${INCLUDES} ${UINTA_CORE_INCLUDE})
set(INCLUDES ${INCLUDES} PARENT_SCOPE)

find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_opengl_LIBRARY})

find_package(glm REQUIRED)
set(INCLUDES ${INCLUDES} ${GLM_INCLUDE})

find_package(assimp REQUIRED)
set(INCLUDES ${INCLUDES} ${ASSIMP_INCLUDE})
set(LIBRARIES ${LIBRARIES} ${ASSIMP_LIB})

add_library(UintaCore STATIC ${SOURCES})

include_directories(${INCLUDES})
target_link_libraries(UintaCore ${LIBRARIES})
