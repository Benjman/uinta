#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_CAMERA_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_CAMERA_UI_H_

#include <imgui.h>

#include "uinta/camera/camera.h"
#include "uinta/camera/camera_manager.h"
#include "uinta/debug/ui/utils.h"
#include "uinta/math/spatial.h"

namespace uinta {

inline f32 wrapFloat(f32 value, f32 min = 0, f32 max = 360) noexcept {
  auto wrapped = fmod(value, max);
  if (wrapped < min) wrapped += max;
  return wrapped;
}

inline void RenderCameraUi(CameraManager* camManager) noexcept {
  if (!ImGui::TreeNode("Camera")) return;

  auto camera = camManager->camera();
  auto flags = camera->flags().as<u32>();
  auto config = camera->config();
  auto updateConfig = false;
  auto updateAgility = false;
  auto updateFlags = false;
  static bool smoothingEnabled = true;
  static i32 projectionSelected = static_cast<i32>(camera->projection());

  auto agility = std::abs(camera->angle().agility());
  auto angle = camera->angle().target();
  auto dist = camera->dist().target();
  auto pitch = camera->pitch().target();
  auto orthoSize = camManager->orthoSize();

  auto forward = getForward(camera->pitch(), camera->angle());
  auto right = getRight(camera->angle());
  auto up = getUp(forward, right);

  std::array<f32, 2> cameraClippingValues = {config.near, config.far};

  updateFlags |= ImGui::CheckboxFlags("Lock Angle", &flags,
                                      CameraConfig::Flags::AngleLockMask);
  ImGui::SameLine();
  updateFlags |= ImGui::CheckboxFlags("Lock Dist", &flags,
                                      CameraConfig::Flags::DistLockMask);
  ImGui::SameLine();
  updateFlags |= ImGui::CheckboxFlags("Lock Pitch", &flags,
                                      CameraConfig::Flags::PitchLockMask);

  updateFlags |= ImGui::CheckboxFlags("Limit##CameraDistLimit", &flags,
                                      CameraConfig::Flags::DistLimitMask);
  ImGui::SameLine();
  if (camera->flags().isDistLimit()) {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing * 2 -
                         UiOneQuartersWidth);
    if (ImGui::SliderScalar("##CameraDist", ImGuiDataType_Float, &dist,
                            &config.dstMin, &config.dstMax, "Distance %.1f"))
      camera->dist(dist);
    ImGui::SameLine();
    ImGui::PushItemWidth(UiOneQuartersWidth);
    updateConfig |= ImGui::DragScalar("##CameraDistMax", ImGuiDataType_Float,
                                      &config.dstMax, 1, &config.dstMin,
                                      &Limits.max, "Max %.0f") ||
                    updateConfig;
    ImGui::PopItemWidth();
  } else {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
    if (ImGui::DragScalar("##CameraDist", ImGuiDataType_Float, &dist, 0.01,
                          &Limits.min, &Limits.max, "Distance %.1f"))
      camera->dist(dist);
  }

  updateFlags |= ImGui::CheckboxFlags("Limit##CameraPitch", &flags,
                                      CameraConfig::Flags::PitchLimitMask);
  ImGui::SameLine();
  if (camera->flags().isPitchLimit()) {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing * 2 -
                         UiOneQuartersWidth);
    if (ImGui::SliderScalar("##CameraPitch", ImGuiDataType_Float, &pitch,
                            &config.pitchMin, &config.pitchMax, "Pitch %.1f"))
      camera->pitch(pitch);
    ImGui::SameLine();
    ImGui::PushItemWidth(UiOneQuartersWidth);
    updateConfig |= ImGui::DragScalar("##CameraPitchMax", ImGuiDataType_Float,
                                      &config.pitchMax, 1, &config.pitchMin,
                                      &Limits.max, "Max %.0f") ||
                    updateConfig;
  } else {
    ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
    if (ImGui::DragScalar("##CameraPitch", ImGuiDataType_Float, &pitch, 0.1,
                          &Limits.min, &Limits.max, "Pitch %.1f"))
      camera->pitch(pitch);
  }
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(UiHalfWidth);
  auto displayAngle = wrapFloat(angle, 0, 360);
  if (ImGui::SliderScalar("##CameraAngle", ImGuiDataType_Float, &displayAngle,
                          &Limits.zero, &Limits.threeSixty, "Angle %.1f")) {
    auto delta = displayAngle - wrapFloat(angle);
    camera->angle(angle + delta);
  }
  ImGui::PopItemWidth();

  ImGui::SameLine();
  ImGui::PushItemWidth(UiHalfWidth - UiSpacing);
  updateConfig |=
      ImGui::DragScalar("##CameraYOff", ImGuiDataType_Float,
                        reinterpret_cast<void*>(&config.vertOff), 0.1f,
                        &Limits.zero, &Limits.max, "Y-Offset %.1f") ||
      updateConfig;
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(UiThreeQuartersWidth);
  updateConfig |=
      ImGui::DragScalar("##CameraSpeedFactor", ImGuiDataType_Float,
                        reinterpret_cast<void*>(&config.trnslScaleFct), 0.005f,
                        &Limits.zero, &Limits.max, "CTSF %.1f") ||
      updateConfig;
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip(
        "Camera Translation Scaling Factor: The closer the camera is to the "
        "target, the slower the camera moves. Conversely, the farther the "
        "camera is from the target, the faster it moves");
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  updateConfig |=
      ImGui::DragScalar("##CameraSpeedFactorMin", ImGuiDataType_Float,
                        reinterpret_cast<void*>(&config.spdFactorMin), 0.005f,
                        &Limits.zero, &Limits.max, "Min %.1f") ||
      updateConfig;
  ImGui::PopItemWidth();

  ImGui::PushItemWidth(UiWidth);
  ImGui::Checkbox("##TweenAgilityDisable", &smoothingEnabled);
  ImGui::SameLine();
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  if (!smoothingEnabled) ImGui::BeginDisabled();
  updateAgility =
      ImGui::DragScalar("##TweenAgility", ImGuiDataType_Float, &agility, 0.1f,
                        &Limits.oneTenth, &Limits.max, "Tween Agility %.1f") ||
      updateAgility;
  if (!smoothingEnabled) ImGui::EndDisabled();
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Camera interpolation smoothing agility");
  ImGui::PopItemWidth();

  ImGui::Text("Clipping Planes");
  ImGui::PushItemWidth(UiWidth - ImGui::GetItemRectSize().x - UiSpacing);
  ImGui::SameLine();
  updateConfig |=
      ImGui::DragFloat2("##CameraClippingPlanes", cameraClippingValues.data(),
                        0.05, config.far, config.near, "%.1f",
                        ImGuiSliderFlags_AlwaysClamp) ||
      updateConfig;
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip(
        "Clipping planes currently not updating correctly until the viewport "
        "has been resized.");
  ImGui::PopItemWidth();

  ImGui::Text("Input");
  ImGui::SameLine();
  updateFlags |= ImGui::CheckboxFlags("Keyboard##CameraInputKeyboard", &flags,
                                      CameraConfig::Flags::KeyboardEnabledMask);
  ImGui::SameLine();
  updateFlags |= ImGui::CheckboxFlags("Mouse##CameraInputMouse", &flags,
                                      CameraConfig::Flags::MouseEnabledMask);

  ImGui::BeginGroup();
  ImGui::Text("Projection");
  ImGui::SameLine();
  if (ImGui::RadioButton("Perspective##CameraProjection", &projectionSelected,
                         static_cast<i32>(CameraProjection::Perspective)))
    camManager->projection(static_cast<CameraProjection>(projectionSelected));
  ImGui::SameLine();
  if (ImGui::RadioButton("Ortho##CameraProjection", &projectionSelected,
                         static_cast<i32>(CameraProjection::Orthographic)))
    camManager->projection(static_cast<CameraProjection>(projectionSelected));
  if (static_cast<CameraProjection>(projectionSelected) ==
      CameraProjection::Orthographic) {
    if (ImGui::SliderScalar("##CameraOrthoSize", ImGuiDataType_Float,
                            &orthoSize, &Limits.one, &Limits.oneHundred,
                            "Ortho Size %.1f"))
      camera->config().orthoSize = orthoSize;
  }
  ImGui::EndGroup();

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

  if (updateAgility) {
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
  }

  if (!smoothingEnabled) camera->force();

  if (updateFlags) camera->flags(flags);
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_UI_CAMERA_UI_H_
