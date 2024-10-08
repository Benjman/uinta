cmake_minimum_required(VERSION 3.18)
project(platform)

set(CMAKE_CXX_STANDARD 23)

list(APPEND PLATFORM_SRCS src/desktop_platform.cc)
list(APPEND PLATFORM_SRCS src/desktop_window.cc)
list(APPEND PLATFORM_SRCS src/gl.cc)
list(APPEND PLATFORM_SRCS src/glfw_platform_api.cc)
list(APPEND PLATFORM_SRCS src/types.cc)
list(APPEND PLATFORM_SRCS src/window.cc)

add_library(platform STATIC ${PLATFORM_SRCS})
add_library(uinta::platform ALIAS platform)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(abseil-cpp)
include(eventpp)
include(GLFW)

target_link_libraries(
  platform
  PUBLIC ${PLATFORM_LIBS}
  PRIVATE ${PLATFORM_PRIVATE_LIBS} ${UINTA_LIBS})

target_include_directories(
  platform
  PUBLIC include
  PRIVATE ${UINTA_INCLUDES})

find_package(GTest)
if(GTest_FOUND)
  add_subdirectory(test)
endif()
