#ifndef UINTA_IMGUI_SRC_CPP
#define UINTA_IMGUI_SRC_CPP

#ifndef IMGUI_API_DISABLED

#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

const char* const UINTA_IMGUI_GLSL_VERSION = "#version 330 core";

#endif  // IMGUI_API_DISABLED
#endif  // UINTA_IMGUI_SRC_CPP
