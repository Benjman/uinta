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
    PUBLIC
      $<BUILD_INTERFACE:${IMGUI_DIR}>
      $<BUILD_INTERFACE:${IMGUI_DIR}/backends>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(FILES
  ${IMGUI_DIR}/imgui.h
  ${IMGUI_DIR}/imconfig.h
  ${IMGUI_DIR}/imgui_internal.h
  ${IMGUI_DIR}/imstb_rectpack.h
  ${IMGUI_DIR}/imstb_textedit.h
  ${IMGUI_DIR}/imstb_truetype.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES
  ${IMGUI_DIR}/backends/imgui_impl_glfw.h
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
  ${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

list(APPEND UINTA_LIBS imgui)
