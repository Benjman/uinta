add_library(platform INTERFACE ${PLATFORM_SRCS})
add_library(uinta::platform ALIAS platform)
target_include_directories(platform INTERFACE include ${UINTA_INCLUDES})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GLFW)

target_link_libraries(platform
  INTERFACE ${PLATFORM_LIBS} ${PLATFORM_PRIVATE_LIBS} ${UINTA_LIBS})
