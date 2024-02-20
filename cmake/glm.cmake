find_package(glm QUIET)

if(glm_FOUND)
  message(STATUS "Found installed GLM: ${glm_VERSION}")
else()
  include(FetchContent)
  message(STATUS "GLM not found locally, fetching from repository...")

  FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
  )

  FetchContent_GetProperties(glm)
  if(NOT glm_POPULATED)
    FetchContent_MakeAvailable(glm)
  endif()
endif()

add_definitions(-DGLM_FORCE_SWIZZLE)

list(APPEND UINTA_LIBS glm::glm)
