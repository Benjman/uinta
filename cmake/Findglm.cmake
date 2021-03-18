find_path(GLM_INCLUDE names "glm/glm.hpp" PATHS "${CMAKE_SOURCE_DIR}/dep/glm")

if (NOT GLM_INCLUDE)
    message(FATAL_ERROR "Failed to locate glm. Possibly haven't run the configuration script?")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glm DEFAULT_MSG GLM_INCLUDE)
