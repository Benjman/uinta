cmake_minimum_required(VERSION 3.17)

project(Uinta)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Create a build directory outside of the source code and call cmake from there:\ncmake -S /path/to/Uinta")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DUINTA_DEBUG=1)

    # Turn off optimization for debug build
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Threads REQUIRED)
set(LIBRARIES ${LIBRARIES} Threads::Threads)

find_path(ENTT_INCLUDE names "entt/entt.hpp" PATHS "${CMAKE_SOURCE_DIR}/dep/entt/src")
set(INCLUDES ${INCLUDES} ${ENTT_INCLUDE})

add_subdirectory(src/UintaPlatform)
add_subdirectory(src/UintaCore)

add_subdirectory(app/UintaDesktop)

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_compile_definitions(UINTA_DEBUG=1)
endif ()
