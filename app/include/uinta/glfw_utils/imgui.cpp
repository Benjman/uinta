#ifndef IMGUI_API_DISABLED
#include <uinta/gl.h>
#include <uinta/glfw_utils/imgui.h>

// FIXME there has to be a better way to point to library source files
#include "../../lib/imgui/backends/imgui_impl_glfw.cpp"
#include "../../lib/imgui/backends/imgui_impl_opengl3.cpp"
#include "../../lib/imgui/imgui.cpp"
#include "../../lib/imgui/imgui_draw.cpp"
#include "../../lib/imgui/imgui_tables.cpp"
#include "../../lib/imgui/imgui_widgets.cpp"
#include "../../lib/implot/implot.cpp"
#include "../../lib/implot/implot_items.cpp"
#endif  // IMGUI_API_DISABLED
