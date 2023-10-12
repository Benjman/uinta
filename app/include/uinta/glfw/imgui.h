#ifndef UINTA_IMGUI_H
#define UINTA_IMGUI_H

#ifndef IMGUI_API_DISABLED

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

// clang-format off
#include <uinta/gl/api.h>
// clang-format on

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

const char* const UINTA_IMGUI_GLSL_VERSION = "#version 330 core";

#endif  // IMGUI_API_DISABLED
#endif  // UINTA_IMGUI_H
