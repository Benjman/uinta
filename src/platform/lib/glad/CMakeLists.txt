cmake_minimum_required(VERSION 3.18)
project(glad)
set_source_files_properties(src/glad.c PROPERTIES LANGUAGE CXX)
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
