#include <cfg.hpp>

// TODO use the preprocessor definition LIB_PATH instead of the relative path
//  I struggled with getting the concatenation of LIB_PATH and the file path to work:
//      #include LIB_PATH##"imgui/imgui.cpp" // <-- no worky
#include "../lib/imgui/imgui.cpp"
#include "../lib/imgui/imgui_draw.cpp"
#include "../lib/imgui/imgui_tables.cpp"
#include "../lib/imgui/imgui_widgets.cpp"
#include "../lib/imgui/backends/imgui_impl_opengl3.cpp"
#include "../lib/imgui/backends/imgui_impl_glfw.cpp"
