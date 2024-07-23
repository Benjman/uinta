find_path(IMGUI_DIR
  NAMES imgui.cpp
  PATHS ${UINTA_LIB_DIR}/imgui
)

find_package(imgui QUIET)

add_library(imgui STATIC
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
)

target_include_directories(imgui
    PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

list(APPEND UINTA_LIBS imgui)
