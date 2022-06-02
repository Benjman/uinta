cmake_minimum_required(VERSION 3.18)
project(uinta)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/module")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


set(UINTA_SRCS ${UINTA_SRCS} 

  ./src/camera.cpp
  ./src/file.cpp
  ./src/font.cpp
  ./src/input.cpp
  ./src/math.cpp
  ./src/mesh.cpp
  ./src/metrics.cpp
  ./src/model.cpp
  ./src/quadtree.cpp
  ./src/runner.cpp
  ./src/shader.cpp

  )


set(UINTA_LIB_PATH        ${CMAKE_BINARY_DIR}/lib/)
set(UINTA_RES_PATH        ${CMAKE_BINARY_DIR}/res/)
set(UINTA_GENERATED_PATH  ${CMAKE_BINARY_DIR}/generated)


set(UINTA_INCLUDES ${UINTA_INCLUDES} ${UINTA_GENERATED_PATH})
configure_file(./cmake/cfg.hpp.in ${UINTA_GENERATED_PATH}/uinta/cfg.hpp)


if ("Debug" STREQUAL "${CMAKE_BUILD_TYPE}")
  add_definitions(-DUINTA_DEBUG)
endif()


# dependencies
find_package(glm REQUIRED)
find_package(imgui REQUIRED) # TODO this should only be required when it's a debug build
find_package(stb REQUIRED)
find_package(GLFW REQUIRED)
add_subdirectory(./lib/entt)
set(UINTA_LIBS ${UINTA_LIBS} EnTT::EnTT)
add_subdirectory(./lib/spdlog)
set(UINTA_LIBS ${UINTA_LIBS} ${SPDLOG_SRCS})
set(UINTA_INCLUDES ${UINTA_INCLUDES} ./lib/spdlog/include)


include_directories(./include ${UINTA_INCLUDES})

add_subdirectory(app)

add_library(uinta STATIC ${UINTA_SRCS})
target_link_libraries(uinta PUBLIC ${UINTA_LIBS})

# testing disabled unless cmake was run with an enabling flag: `cmake -DUINTA_TEST=true ..`
if (UINTA_TEST)
  add_subdirectory(test) # FIXME issues with tests not building...
endif()

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
