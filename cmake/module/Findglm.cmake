find_path(GLM_INCLUDE names "glm/glm.hpp" PATHS "${CMAKE_SOURCE_DIR}/lib/glm")
set(UINTA_INCLUDES ${UINTA_INCLUDES} ${GLM_INCLUDE})

if (NOT GLM_INCLUDE)
    message(FATAL_ERROR "could not locate glm/glm.hpp in ${CMAKE_SOURCE_DIR}/lib/glm")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glm DEFAULT_MSG GLM_INCLUDE)
