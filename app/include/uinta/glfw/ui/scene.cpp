#include <uinta/glfw/ui/scene/lighting.cpp>
#include <uinta/runner.hpp>

namespace uinta {

inline void scene(Runner& runner) {
  if (!ImGui::CollapsingHeader("Scene")) return;
  lighting(runner.scene);
}

}  // namespace uinta
