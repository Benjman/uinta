find_package(glm QUIET)
if(glm_FOUND)
  message(STATUS "Found local GLM: ${glm_VERSION}")
else()
  include(FetchContent)
  message(STATUS "GLM not found locally, fetching from repository...")

  FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3
  )
  FetchContent_MakeAvailable(glm)
endif()
add_definitions(-DGLM_FORCE_SWIZZLE)
list(APPEND PLATFORM_LIBS glm::glm)
