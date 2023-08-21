#include <uinta/glfw/imgui.h>
#include <uinta/glfw/ui/limits.h>

#include <uinta/camera/target_camera.hpp>
#include <uinta/math/utils.hpp>

namespace uinta {

inline void camera(TargetCamera &camera);
inline void cameraClippingPlanes(TargetCamera &camera);
inline void cameraHotkeys(TargetCamera &camera);
inline void cameraTransform(TargetCamera &camera);

inline void camera(TargetCamera &camera) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::CollapsingHeader("Camera")) return;
  ImGui::PushItemWidth(200);
  cameraClippingPlanes(camera);
  cameraHotkeys(camera);
  cameraTransform(camera);
  ImGui::PopItemWidth();
#endif  // IMGUI_API_DISABLED
}

inline void cameraClippingPlanes(TargetCamera &camera) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::TreeNode("Clipping planes")) return;
  f32 cameraClippingValues[] = {camera.config.nearPlane, camera.config.farPlane};
  ImGui::DragFloat2("Clipping planes", cameraClippingValues, 0.05, camera.config.farPlane, camera.config.nearPlane, "%+.1f",
                    ImGuiSliderFlags_AlwaysClamp);
  camera.config.nearPlane = cameraClippingValues[0];
  camera.config.farPlane = cameraClippingValues[1];
  ImGui::TreePop();
  ImGui::Separator();
#endif  // IMGUI_API_DISABLED
}

inline void cameraHotkeys(TargetCamera &camera) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::TreeNode("Hotkeys")) return;
  ImGui::Text("Translation:   edsf or LMB");
  ImGui::Text("Angle:         wr or RMB");
  ImGui::Text("Pitch:         qa or RMB");
  ImGui::Text("Dist:          cv or Scroll");

  ImGui::TreePop();
  ImGui::Separator();
#endif  // IMGUI_API_DISABLED
}

inline void cameraTransform(TargetCamera &camera) {
#ifndef IMGUI_API_DISABLED
  if (!ImGui::TreeNode("Transform")) return;
  f32 agility = camera.angle.agility;
  if (ImGui::DragScalar("Translation agility", ImGuiDataType_Float, reinterpret_cast<void *>(&agility), 0.1f, &limits.min,
                        &limits.max, "%+.2f")) {
    camera.dist.agility = agility;
    camera.pitch.agility = agility;
    camera.target.x.agility = agility;
    camera.target.y.agility = agility;
    camera.target.z.agility = agility;
  }

  ImGui::DragScalar("Dist", ImGuiDataType_Float, reinterpret_cast<void *>(&camera.dist.target), 0.1f, &limits.one_tenth,
                    &limits.twenty, "%+.2f");
  ImGui::SameLine();
  ImGui::CheckboxFlags("Limit dist", (u32 *)&camera.config.flags, CAMERA_DIST_LIMIT);

  ImGui::DragScalar("Pitch", ImGuiDataType_Float, reinterpret_cast<void *>(&camera.pitch.target), 0.1f, &limits.min, &limits.max,
                    "%+.2f");
  ImGui::SameLine();
  ImGui::CheckboxFlags("Limit pitch", (u32 *)&camera.config.flags, CAMERA_PITCH_LIMIT);

  ImGui::DragScalar("Angle", ImGuiDataType_Float, reinterpret_cast<void *>(&camera.angle.target), 0.1f, &limits.min, &limits.max,
                    "%+.2f");

  ImGui::DragScalar("Y-Offset", ImGuiDataType_Float, reinterpret_cast<void *>(&camera.vertOffset), 0.1f, &limits.zero,
                    &limits.max, "%+.2f");

  ImGui::DragScalar("Speed factor", ImGuiDataType_Float, reinterpret_cast<void *>(&camera.config.translationSpeedDistFactor),
                    0.005f, &limits.zero, &limits.max, "%+.2f");
  ImGui::DragScalar("Speed factor min", ImGuiDataType_Float,
                    reinterpret_cast<void *>(&camera.config.translationSpeedDistFactorMin), 0.005f, &limits.zero, &limits.max,
                    "%+.2f");
  ImGui::Text("Speed scalar  %+.2f", calculateTranslationFactor(camera));

  auto forward = getForward(camera.pitch, camera.angle);
  auto right = getRight(camera.angle);
  auto up = getUp(forward, right);
  ImGui::Text("Position      %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
  ImGui::Text("Target        %+.2f %+.2f %+.2f", camera.target.x.target, camera.target.y.target, camera.target.z.target);
  ImGui::Text("Forward       %+.2f %+.2f %+.2f", forward.x, forward.y, forward.z);
  ImGui::Text("Right         %+.2f %+.2f %+.2f", right.x, right.y, right.z);
  ImGui::Text("Up            %+.2f %+.2f %+.2f", up.x, up.y, up.z);

  ImGui::TreePop();
  ImGui::Separator();
#endif  // IMGUI_API_DISABLED
}

}  // namespace uinta
