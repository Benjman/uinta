#ifndef UINTA_IMGUI_SRC_CPP
#define UINTA_IMGUI_SRC_CPP

#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#ifndef IMGUI_API_DISABLED
#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#ifdef UINTA_IMGUI_IMPL
#include <uinta/gl.h>
// FIXME there has to be a better way to point to library source files
#include "../../lib/imgui/backends/imgui_impl_glfw.cpp"
#include "../../lib/imgui/backends/imgui_impl_opengl3.cpp"
#include "../../lib/imgui/imgui.cpp"
#include "../../lib/imgui/imgui_draw.cpp"
#include "../../lib/imgui/imgui_tables.cpp"
#include "../../lib/imgui/imgui_widgets.cpp"

#endif  // UINTA_IMGUI_IMPL
#endif  // IMGUI_API_DISABLED
#endif  // UINTA_IMGUI_SRC_CPP
