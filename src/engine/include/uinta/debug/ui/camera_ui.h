#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_CAMERA_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_CAMERA_UI_H_

#include <imgui.h>

#include "uinta/camera/camera.h"
#include "uinta/debug/ui/utils.h"
#include "uinta/math/spatial.h"

namespace uinta {

inline f32 wrapAngle(f32 angle) noexcept {
  auto wrapped = fmod(angle, 360.0);
  if (wrapped < 0) wrapped += 360;
  return wrapped;
}

inline void RenderCameraUi(Camera* camera) noexcept {
  if (!ImGui::TreeNode("Camera")) return;

  auto flags = camera->flags().value<u32>();
  auto config = camera->config();
  auto updateConfig = false;

  auto agility = std::abs(camera->angle().agility());
  auto angle = camera->angle().target();
  auto dist = camera->dist().target();
  auto pitch = camera->pitch().target();

  auto forward = getForward(camera->pitch(), camera->angle());
  auto right = getRight(camera->angle());
  auto up = getUp(forward, right);

  std::array<f32, 2> cameraClippingValues = {config.near, config.far};

  auto onTranslationAgilityChange = [&]() {
    auto angle = camera->angle();
    angle.agility(agility);
    camera->angle(angle);

    auto dist = camera->dist();
    dist.agility(agility);
    camera->dist(dist);

    auto pitch = camera->pitch();
    pitch.agility(agility);
    camera->pitch(pitch);

    auto target = camera->target();
    target.agility(agility);
    camera->target(target);
  };

  ImGui::PushID("CameraDist");
  if (ImGui::CheckboxFlags("Limit", &flags, Camera::Flags::DistLimitMask))
    camera->flags(static_cast<Camera::Flags::value_type>(flags));
  ImGui::SameLine();
  if (camera->flags().isDistLimit()) {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing * 2 -
                         UiOneQuartersWidth);
    if (ImGui::SliderScalar("", ImGuiDataType_Float, &dist, &config.dstMin,
                            &config.dstMax, "Distance %.1f"))
      camera->dist(dist);
    ImGui::SameLine();
    ImGui::PushID("Max");
    ImGui::PushItemWidth(UiOneQuartersWidth);
    updateConfig = ImGui::DragScalar("", ImGuiDataType_Float, &config.dstMax, 1,
                                     &config.dstMin, &Limits.max, "Max %.0f") ||
                   updateConfig;
    ImGui::PopItemWidth();
    ImGui::PopID();
  } else {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
    if (ImGui::DragScalar("", ImGuiDataType_Float, &dist, 0.01, &Limits.min,
                          &Limits.max, "Distance %.1f"))
      camera->dist(dist);
  }
  ImGui::PopID();

  ImGui::PushID("CameraPitch");
  if (ImGui::CheckboxFlags("Limit", &flags, Camera::Flags::PitchLimitMask))
    camera->flags(static_cast<Camera::Flags::value_type>(flags));
  ImGui::SameLine();
  if (camera->flags().isPitchLimit()) {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing * 2 -
                         UiOneQuartersWidth);
    if (ImGui::SliderScalar("", ImGuiDataType_Float, &pitch, &config.pitchMin,
                            &config.pitchMax, "Pitch %.1f"))
      camera->pitch(pitch);
    ImGui::SameLine();
    ImGui::PushID("Max");
    ImGui::PushItemWidth(UiOneQuartersWidth);
    updateConfig =
        ImGui::DragScalar("", ImGuiDataType_Float, &config.pitchMax, 1,
                          &config.pitchMin, &Limits.max, "Max %.0f") ||
        updateConfig;
    ImGui::PopID();
  } else {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
    if (ImGui::DragScalar("", ImGuiDataType_Float, &pitch, 0.1, &Limits.min,
                          &Limits.max, "Pitch %.1f"))
      camera->pitch(pitch);
  }
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::PushID("CameraAngle");
  ImGui::PushItemWidth(UiHalfWidth);
  auto displayAngle = wrapAngle(angle);
  if (ImGui::DragScalar("", ImGuiDataType_Float, &displayAngle, 0.1f,
                        &Limits.min, &Limits.max, "Angle %.1f")) {
    auto delta = displayAngle - wrapAngle(angle);
    camera->angle(angle + delta);
  }
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::SameLine();
  ImGui::PushID("CameraYOff");
  ImGui::PushItemWidth(UiHalfWidth - UiSpacing);
  updateConfig =
      ImGui::DragScalar("", ImGuiDataType_Float,
                        reinterpret_cast<void*>(&config.vertOff), 0.1f,
                        &Limits.zero, &Limits.max, "Y-Offset %.1f") ||
      updateConfig;
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::PushID("CameraSpeedFactor");
  ImGui::PushItemWidth(UiThreeQuartersWidth);
  updateConfig =
      ImGui::DragScalar("", ImGuiDataType_Float,
                        reinterpret_cast<void*>(&config.trnslScaleFct), 0.005f,
                        &Limits.zero, &Limits.max, "CTFS %.1f") ||
      updateConfig;
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip(
        "Camera Translation Scaling Factor: The closer the camera is to the "
        "target, the slower the camera moves. Conversely, the farther the "
        "camera is from the target, the faster it moves");
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::PopID();

  ImGui::PushID("CameraSpeedFactorMin");
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  updateConfig =
      ImGui::DragScalar("", ImGuiDataType_Float,
                        reinterpret_cast<void*>(&config.spdFactorMin), 0.005f,
                        &Limits.zero, &Limits.max, "Min %.1f") ||
      updateConfig;
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::PushID("TweenAgility");
  ImGui::PushItemWidth(UiWidth);
  if (ImGui::DragScalar("", ImGuiDataType_Float, &agility, 0.1f,
                        &Limits.one_tenth, &Limits.max, "Tween Agility %.1f"))
    onTranslationAgilityChange();
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Camera interpolation smoothing agility");
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::PushID("CameraClippingPlanes");
  ImGui::Text("Clipping Planes");
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  ImGui::SameLine();
  updateConfig =
      ImGui::DragFloat2("", cameraClippingValues.data(), 0.05, config.far,
                        config.near, "%.1f", ImGuiSliderFlags_AlwaysClamp) ||
      updateConfig;
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip(
        "Clipping planes currently not updating correctly until the viewport "
        "has been resized.");
  ImGui::PopItemWidth();
  ImGui::PopID();

  ImGui::Text("%-25s%+.1f", "Speed Scalar", camera->translationFactor());
  ImGui::Text("%-25s%+.1f %+.1f %+.1f", "Position", camera->position().x,
              camera->position().y, camera->position().z);
  ImGui::Text("%-25s%+.1f %+.1f %+.1f", "Target", camera->target().x().target(),
              camera->target().y().target(), camera->target().z().target());
  ImGui::Text("%-25s%+.1f %+.1f %+.1f", "Forward", forward.x, forward.y,
              forward.z);
  ImGui::Text("%-25s%+.1f %+.1f %+.1f", "Right", right.x, right.y, right.z);
  ImGui::Text("%-25s%+.1f %+.1f %+.1f", "Up", up.x, up.y, up.z);

  ImGui::TreePop();

  if (updateConfig) {
    config.near = cameraClippingValues.at(0);
    config.far = cameraClippingValues.at(1);
    camera->config(config);
  }
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_CAMERA_UI_H_
