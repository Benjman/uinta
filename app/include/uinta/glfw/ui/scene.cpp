#include <uinta/glfw/imgui.h>

#include <uinta/runner.hpp>

namespace uinta {

inline void scene(Runner& runner) {
  if (!ImGui::CollapsingHeader("Scene")) return;
}

}  // namespace uinta
