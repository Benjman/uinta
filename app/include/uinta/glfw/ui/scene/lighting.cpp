#include <uinta/glfw/imgui.h>
#include <uinta/glfw/ui/limits.h>

#include <glm/gtc/type_ptr.hpp>
#include <uinta/scene/scene.hpp>

namespace uinta {

inline void diffuseLighting(Scene& scene) {
  static f32 diffuseDirBuffer[3];
  const auto& light = scene.diffuse_light();
  memcpy(diffuseDirBuffer, glm::value_ptr(light.direction), 3 * sizeof(typeof(light.direction.x)));
  if (ImGui::SliderFloat3("Diffuse lighting", diffuseDirBuffer, -limits.one, limits.one))
    scene.diffuse_light({{diffuseDirBuffer[0], diffuseDirBuffer[1], diffuseDirBuffer[2]}});
  ImGui::Text("normalized(%+.2f %+.2f %+.2f)", light.direction.x, light.direction.y, light.direction.z);
}

inline void lighting(Scene& scene) {
  if (!ImGui::TreeNode("Lighting")) return;
  diffuseLighting(scene);
  ImGui::TreePop();
  ImGui::Separator();
}

}  // namespace uinta
