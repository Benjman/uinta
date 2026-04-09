find_package(absl QUIET)

if(absl_FOUND)
  message(STATUS "Found local Abseil: ${absl_VERSION}")
else()
  include(FetchContent)
  message(STATUS "Abseil not found locally, fetching from repository...")

  FetchContent_Declare(
    absl
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG        20260107.1
  )

  FetchContent_GetProperties(absl)
  if(NOT absl_POPULATED)
    set(ABSL_PROPAGATE_CXX_STD ON)
    option(ABSL_USE_SYSTEM_INCLUDES ON)
    FetchContent_MakeAvailable(absl)
  endif()
endif()

set(UINTA_ABSL
  absl::log
  absl::random_random
  absl::status
  absl::statusor
  CACHE INTERNAL "Abseil libraries")

list(APPEND UINTA_LIBS ${UINTA_ABSL})
