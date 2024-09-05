cmake_minimum_required(VERSION 3.18)
project(engine)

set(CMAKE_CXX_STANDARD 20)

list(APPEND ENGINE_SRCS src/engine.cc)
list(APPEND ENGINE_SRCS src/reflect.cc)

add_library(engine STATIC ${ENGINE_SRCS})
add_library(uinta::engine ALIAS engine)
target_include_directories(
  engine
  PUBLIC include
  PRIVATE ${UINTA_INCLUDES})
list(APPEND ENGINE_LIBS uinta::platform)
target_link_libraries(engine PRIVATE ${ENGINE_LIBS} ${UINTA_LIBS})

set(UINTA_ENGINE_RES "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(UINTA_ENGINE_RES ${UINTA_ENGINE_RES} PARENT_SCOPE)
list(APPEND UINTA_RES_PATHS "${UINTA_ENGINE_RES}")
set(UINTA_RES_PATHS ${UINTA_RES_PATHS} PARENT_SCOPE)

find_package(GTest)
if(GTest_FOUND)
  add_subdirectory(test)
endif()
