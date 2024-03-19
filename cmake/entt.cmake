find_package(EnTT QUIET)

if(EnTT_FOUND)
  message(STATUS "Found installed EnTT: ${EnTT_VERSION}")
else()
  include(FetchContent)
  message(STATUS "EnTT not found locally, fetching from repository...")

  FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.13.1
  )

  FetchContent_GetProperties(EnTT)
  if(NOT EnTT_POPULATED)
    option(ENTT_USE_LIBCPP ON)
    option(ENTT_USE_CLANG_TIDY ON)
    FetchContent_MakeAvailable(EnTT)
  endif()
endif()

list(APPEND UINTA_LIBS EnTT::EnTT)
