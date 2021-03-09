cmake_minimum_required(VERSION 3.17)

project(UintaCore
        VERSION 0.1)

set(SOURCES ${SOURCES}
        src/io/file.cpp
        src/shader/shader.cpp
        )

set(UINTA_CORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(UINTA_CORE_INCLUDE ${UINTA_CORE_INCLUDE} PARENT_SCOPE)

set(INCLUDES ${INCLUDES} ${UINTA_CORE_INCLUDE})
set(INCLUDES ${INCLUDES} PARENT_SCOPE)

find_package(OpenGL REQUIRED)
set(LIBRARIES ${LIBRARIES} ${OPENGL_opengl_LIBRARY})

include_directories(${INCLUDES})
add_library(UintaCore STATIC ${SOURCES})

target_link_libraries(UintaCore ${LIBRARIES})
