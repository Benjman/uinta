# Check if eventpp submodule exists
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/eventpp/CMakeLists.txt")
  message(FATAL_ERROR
    "eventpp submodule not found at ${CMAKE_CURRENT_SOURCE_DIR}/lib/eventpp. "
    "Initialize git submodules with: git submodule update --init --recursive")
endif()

add_subdirectory(lib/eventpp)
list(APPEND PLATFORM_LIBS eventpp::eventpp)
