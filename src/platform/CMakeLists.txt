add_library(platform INTERFACE ${PLATFORM_SRCS})
add_library(uinta::platform ALIAS platform)
target_include_directories(platform INTERFACE include ${UINTA_INCLUDES})

target_link_libraries(
  platform
  PUBLIC ${PLATFORM_LIBS}
  PRIVATE ${PLATFORM_PRIVATE_LIBS} ${UINTA_LIBS})
