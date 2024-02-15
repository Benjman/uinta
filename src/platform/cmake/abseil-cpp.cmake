find_package(absl QUIET)

if(absl_FOUND)
  message(STATUS "Found local Abseil: ${absl_VERSION}")
else()
  include(FetchContent)
  message(STATUS "Abseil not found locally, fetching from repository...")

  FetchContent_Declare(
    absl
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG        20240116.1
  )

  FetchContent_GetProperties(absl)
  if(NOT absl_POPULATED)
    set(ABSL_PROPAGATE_CXX_STD ON)
    option(ABSL_USE_SYSTEM_INCLUDES ON)
    FetchContent_MakeAvailable(absl)
  endif()
endif()

list(APPEND UINTA_ABSL absl::log)
list(APPEND UINTA_ABSL absl::status)
list(APPEND UINTA_ABSL absl::statusor)

list(APPEND UINTA_LIBS ${UINTA_ABSL})
