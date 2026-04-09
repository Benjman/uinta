find_package(yaml-cpp REQUIRED)

if(yaml-cpp_FOUND)
  message(STATUS "Found installed yaml-cpp: ${yaml-cpp_VERSION} -- ${YAML_CPP_LIBRARY_DIR}")
else()
  include(FetchContent)
  message(STATUS "yaml-cpp not found locally, fetching from repository...")

  FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG        yaml-cpp-0.7.0
  )

  FetchContent_GetProperties(yaml-cpp)
  if(NOT yaml-cpp_POPULATED)
    FetchContent_MakeAvailable(yaml-cpp)
  endif()
endif()

list(APPEND PLATFORM_LIBS ${YAML_CPP_LIBRARIES})
