cmake_minimum_required(VERSION 3.15)

project(UintaCore
        VERSION 0.1)

set(SOURCES ${SOURCES}
        src/gl/gl_error.cpp
        src/gl/gl_state.cpp
        src/io/file.cpp
        src/shader/shader.cpp
        src/shader/vao.cpp
        src/shader/vbo.cpp
        src/shader/vertex_attribute.cpp
        src/text/font.cpp
        src/texture/texture.cpp
        )

set(UINTA_CORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(UINTA_CORE_INCLUDE ${UINTA_CORE_INCLUDE} PARENT_SCOPE)

set(INCLUDES ${INCLUDES} ${UINTA_CORE_INCLUDE})
set(INCLUDES ${INCLUDES} PARENT_SCOPE)

find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_opengl_LIBRARY})

find_package(stb REQUIRED)
set(INCLUDES ${INCLUDES} ${STB_INCLUDE})

include_directories(${INCLUDES})
add_library(UintaCore STATIC ${SOURCES})

target_link_libraries(UintaCore ${LIBRARIES})
