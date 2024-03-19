find_package(assimp QUIET)

if(assimp_FOUND)
  message(STATUS "Found installed Assimp: ${assimp_VERSION}")
else()
  include(FetchContent)
  message(STATUS "Assimp not found locally, fetching from repository...")

  FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.3.1
  )

  FetchContent_GetProperties(assimp)
  if(NOT assimp_POPULATED)
    option(ASSIMP_INSTALL OFF)
    option(ASSIMP_BUILD_TESTS OFF)
    option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
    FetchContent_MakeAvailable(assimp)
  endif()
endif()

list(APPEND UINTA_LIBS assimp)
