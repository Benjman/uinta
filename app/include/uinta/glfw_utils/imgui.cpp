#ifndef IMGUI_API_DISABLED
#include <uinta/gl.h>
#include <uinta/glfw_utils/imgui.h>

// FIXME: These should really be linked through a static library. Amend this line_blame commit when done.
#include "../../lib/imgui/backends/imgui_impl_glfw.cpp"
#include "../../lib/imgui/backends/imgui_impl_opengl3.cpp"
#include "../../lib/imgui/imgui.cpp"
#include "../../lib/imgui/imgui_draw.cpp"
#include "../../lib/imgui/imgui_tables.cpp"
#include "../../lib/imgui/imgui_widgets.cpp"
#include "../../lib/implot/implot.cpp"
#include "../../lib/implot/implot_items.cpp"
#endif  // IMGUI_API_DISABLED
