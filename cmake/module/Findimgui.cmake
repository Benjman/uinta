find_path(IMGUI_INCLUDE names "imgui.h" PATHS "${CMAKE_SOURCE_DIR}/lib/imgui")
set(UINTA_INCLUDES ${UINTA_INCLUDES} ${IMGUI_INCLUDE})

if (NOT IMGUI_INCLUDE)
    message(FATAL_ERROR "could not locate imgui/imgui.h in ${CMAKE_SOURCE_DIR}/lib/imgui")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(imgui DEFAULT_MSG IMGUI_INCLUDE)
