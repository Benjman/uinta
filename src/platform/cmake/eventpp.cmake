find_package(eventpp REQUIRED)
if(eventpp_FOUND)
  message(STATUS "Found installed eventpp: ${eventpp_VERSION}")
else()
  # TODO: Fetch if not available

  # If you've pulled down the submodule, simply `make install` it
  message(FATAL_ERROR "Failed to locate eventpp library. Please install it and
    make it available on the path.")
endif()
list(APPEND PLATFORM_LIBS eventpp::eventpp)
