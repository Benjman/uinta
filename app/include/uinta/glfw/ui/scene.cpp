#include <uinta/glfw/imgui.h>
#include <uinta/glfw/ui/limits.h>

#include <glm/gtc/type_ptr.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>

namespace uinta {

inline void scene(Runner& runner) {
  if (!ImGui::CollapsingHeader("Scene")) return;
  if (ImGui::TreeNode("Lighting")) {
    static bool hasInitialized = false;
    static f32 diffuseDirBuffer[3];
    const auto& light = runner.scene.getDiffuseLight();
    if (!hasInitialized) {
      memcpy(diffuseDirBuffer, glm::value_ptr(light.direction), 3 * sizeof(typeof(light.direction.x)));
      hasInitialized = true;
    }
    if (ImGui::SliderFloat3("Diffuse lighting", diffuseDirBuffer, -limits.twenty, limits.twenty))
      runner.scene.updateDiffuseLight({{diffuseDirBuffer[0], diffuseDirBuffer[1], diffuseDirBuffer[2]}});
    ImGui::Text("normalized(%+.2f %+.2f %+.2f)", light.direction.x, light.direction.y, light.direction.z);
    ImGui::TreePop();
    ImGui::Separator();
  }
}

}  // namespace uinta
