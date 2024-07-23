find_package(imgui QUIET)

if(imgui_FOUND)
  message(STATUS "Found installed imgui: ${imgui_VERSION}")
else()
  include(FetchContent)
  message(STATUS "Dear ImGui not found locally, fetching from repository...")

  FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.7
  )

  FetchContent_GetProperties(imgui)
  if(NOT imgui_POPULATED)
    FetchContent_MakeAvailable(imgui)
  endif()

  add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  )

  target_include_directories(imgui
    PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

endif()

list(APPEND UINTA_LIBS imgui)
