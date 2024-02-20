find_package(glm REQUIRED)
message(STATUS "Found installed GLM: ${glm_VERSION}")

add_definitions(-DGLM_FORCE_SWIZZLE)

list(APPEND UINTA_LIBS glm::glm)