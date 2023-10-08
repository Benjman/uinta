#include <uinta/glfw/imgui.h>
#include <uinta/glfw/ui/limits.h>

#include <uinta/math/direction.hpp>
#include <uinta/runner_state.hpp>
#include <uinta/target_camera.hpp>

namespace uinta {
namespace ui {

inline bool cameraClippingPlanes(TargetCamera &camera);
inline bool cameraHotkeys(TargetCamera &camera);
inline bool cameraTransform(TargetCamera &camera, const RunnerState &state);

inline bool camera(TargetCamera &camera, const RunnerState &state) {
#ifndef IMGUI_API_DISABLED
  ImGui::PushItemWidth(200);
  auto cpUpdated = cameraClippingPlanes(camera);
  auto hkUpdated = cameraHotkeys(camera);
  auto tfUpdated = cameraTransform(camera, state);
  ImGui::PopItemWidth();
#endif  // IMGUI_API_DISABLED
  return cpUpdated || hkUpdated || tfUpdated;
}

inline bool cameraClippingPlanes(TargetCamera &camera) {
  bool result = false;
#ifndef IMGUI_API_DISABLED
  if (ImGui::TreeNode("Clipping planes")) {
    f32 cameraClippingValues[] = {camera.config().near, camera.config().far};
    if (ImGui::DragFloat2("Clipping planes", cameraClippingValues, 0.05, camera.config().far, camera.config().near, "%+.1f",
                          ImGuiSliderFlags_AlwaysClamp))
      result = true;
    auto config = camera.config();
    config.near = cameraClippingValues[0];
    config.far = cameraClippingValues[1];
    camera.config(config);
    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
  return result;
}

inline bool cameraHotkeys(TargetCamera &camera) {
#ifndef IMGUI_API_DISABLED
  if (ImGui::TreeNode("Hotkeys")) {
    ImGui::Text("Translation:   edsf or LMB");
    ImGui::Text("Angle:         wr or RMB");
    ImGui::Text("Pitch:         qa or RMB");
    ImGui::Text("Dist:          cv or Scroll");

    ImGui::TreePop();
    ImGui::Separator();
  }
#endif  // IMGUI_API_DISABLED
  return false;
}

inline bool cameraTransform(TargetCamera &camera, const RunnerState &state) {
  bool result = false;
#ifndef IMGUI_API_DISABLED
  if (ImGui::TreeNode("Transform")) {
    auto agility = camera.angle().agility();
    if (ImGui::DragScalar("Translation agility", ImGuiDataType_Float, reinterpret_cast<void *>(&agility), 0.1f, &limits.min,
                          &limits.max, "%+.2f")) {
      auto angle = camera.angle();
      angle.agility(agility);
      camera.angle(angle);

      auto dist = camera.dist();
      dist.agility(agility);
      camera.dist(dist);

      auto pitch = camera.pitch();
      pitch.agility(agility);
      camera.pitch(pitch);

      auto target = camera.target();
      target.agility(agility);
      camera.target(target);

      result = true;
    }

    auto dist = camera.dist().target();
    if (ImGui::DragScalar("Dist", ImGuiDataType_Float, reinterpret_cast<void *>(&dist), 0.1f, &limits.one_tenth, &limits.twenty,
                          "%+.2f")) {
      camera.dist(dist);
      result = true;
    }

    ImGui::SameLine();

    auto flags = camera.flags();
    if (ImGui::CheckboxFlags("Limit dist", (u32 *)&flags, TargetCamera::CAMERA_DIST_LIMIT)) {
      camera.flags() = flags;
      result = true;
    }

    auto pitch = camera.pitch().target();
    if (ImGui::DragScalar("Pitch", ImGuiDataType_Float, reinterpret_cast<void *>(&pitch), 0.1f, &limits.min, &limits.max,
                          "%+.2f")) {
      camera.pitch(pitch);
      result = true;
    }

    ImGui::SameLine();
    if (ImGui::CheckboxFlags("Limit pitch", (u32 *)&flags, TargetCamera::CAMERA_PITCH_LIMIT)) {
      camera.flags() = flags;
      result = true;
    }

    auto angle = camera.angle().target();
    if (ImGui::DragScalar("Angle", ImGuiDataType_Float, reinterpret_cast<void *>(&angle), 0.1f, &limits.min, &limits.max,
                          "%+.2f")) {
      camera.angle(angle);
      result = true;
    }

    auto vert_offset = camera.vert_offset();
    if (ImGui::DragScalar("Y-Offset", ImGuiDataType_Float, reinterpret_cast<void *>(&vert_offset), 0.1f, &limits.zero,
                          &limits.max, "%+.2f")) {
      camera.vert_offset(vert_offset);
      result = true;
    }

    auto spd_factor = camera.config().spd_factor;
    if (ImGui::DragScalar("Speed factor", ImGuiDataType_Float, reinterpret_cast<void *>(&spd_factor), 0.005f, &limits.zero,
                          &limits.max, "%+.2f")) {
      auto config = camera.config();
      config.spd_factor = spd_factor;
      camera.config(config);
      result = true;
    }

    auto spd_factor_min = camera.config().spd_factor_min;
    if (ImGui::DragScalar("Speed factor min", ImGuiDataType_Float, reinterpret_cast<void *>(&spd_factor_min), 0.005f,
                          &limits.zero, &limits.max, "%+.2f")) {
      auto config = camera.config();
      config.spd_factor_min = spd_factor_min;
      camera.config(config);
      result = true;
    }

    static bool doSpin = false;
    static i32 doSpinRate = 15;
    ImGui::Checkbox("Spin", &doSpin);
    if (doSpin) {
      ImGui::SameLine();
      if (ImGui::SliderInt("Rate", &doSpinRate, limits.zero, limits.threeSixty)) result = true;
      camera.angle(camera.angle().target() + doSpinRate * state.delta);
      result = true;
    }

    ImGui::Text("Speed scalar  %+.2f", camera.translation_factor());

    const auto forward = getForward(camera.pitch(), camera.angle());
    const auto right = getRight(camera.angle());
    const auto up = getUp(forward, right);
    ImGui::Text("Position      %+.2f %+.2f %+.2f", camera.position().x, camera.position().y, camera.position().z);
    ImGui::Text("Target        %+.2f %+.2f %+.2f", camera.target().x().target(), camera.target().y().target(),
                camera.target().z().target());
    ImGui::Text("Forward       %+.2f %+.2f %+.2f", forward.x, forward.y, forward.z);
    ImGui::Text("Right         %+.2f %+.2f %+.2f", right.x, right.y, right.z);
    ImGui::Text("Up            %+.2f %+.2f %+.2f", up.x, up.y, up.z);

    ImGui::TreePop();
    ImGui::Separator();
#endif  // IMGUI_API_DISABLED
  }
  return result;
}

}  // namespace ui
}  // namespace uinta
