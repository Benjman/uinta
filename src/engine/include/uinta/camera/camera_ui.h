#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_UI_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_UI_H_

#include "./imgui.h"
#include "uinta/camera/camera.h"
#include "uinta/debug/ui/limits.h"
#include "uinta/engine_state.h"
#include "uinta/math/spatial.h"

namespace uinta {

class CameraUi {
 public:
  explicit CameraUi(Camera* camera) : camera_(camera) {}
  CameraUi() = default;
  CameraUi(const CameraUi&) noexcept = delete;
  CameraUi& operator=(const CameraUi&) noexcept = delete;
  CameraUi(CameraUi&&) noexcept = delete;
  CameraUi& operator=(CameraUi&&) noexcept = delete;

  void render(const EngineState& state) noexcept {
    clippingPlanes();
    transform(state);
  }

 private:
  Camera* camera_;

  void clippingPlanes() const noexcept {
    if (ImGui::TreeNode("Clipping planes")) {
      std::array<f32, 2> cameraClippingValues = {camera_->config().near,
                                                 camera_->config().far};
      ImGui::DragFloat2("Clipping planes", cameraClippingValues.data(), 0.05,
                        camera_->config().far, camera_->config().near, "%+.1f",
                        ImGuiSliderFlags_AlwaysClamp);
      ImGui::TreePop();

      if (camera_->config().near != cameraClippingValues[0] ||
          camera_->config().far != cameraClippingValues[1]) {
        auto config = camera_->config();
        config.near = cameraClippingValues[0];
        config.far = cameraClippingValues[1];
        camera_->config(config);
      }
    }
  }

  void transform(const EngineState& state) const noexcept {
    if (ImGui::TreeNode("Transform")) {
      auto config = camera_->config();
      bool updateConfig = false;
      auto agility = camera_->angle().agility();
      if (ImGui::DragScalar("Translation agility", ImGuiDataType_Float,
                            reinterpret_cast<void*>(&agility), 0.1f,
                            &limits.min, &limits.max, "%+.2f")) {
        auto angle = camera_->angle();
        angle.agility(agility);
        camera_->angle(angle);

        auto dist = camera_->dist();
        dist.agility(agility);
        camera_->dist(dist);

        auto pitch = camera_->pitch();
        pitch.agility(agility);
        camera_->pitch(pitch);

        auto target = camera_->target();
        target.agility(agility);
        camera_->target(target);
      }

      auto dist = camera_->dist().target();
      if (ImGui::DragScalar("Dist", ImGuiDataType_Float,
                            reinterpret_cast<void*>(&dist), 0.1f,
                            &limits.one_tenth, &limits.twenty, "%+.2f")) {
        camera_->dist(dist);
      }

      ImGui::SameLine();

      auto flags = camera_->flags();
      if (ImGui::CheckboxFlags("Limit dist", static_cast<u32*>(&flags),
                               Camera::Masks::DistLimit)) {
        camera_->flags() = flags;
      }

      auto pitch = camera_->pitch().target();
      if (ImGui::DragScalar("Pitch", ImGuiDataType_Float,
                            reinterpret_cast<void*>(&pitch), 0.1f, &limits.min,
                            &limits.max, "%+.2f")) {
        camera_->pitch(pitch);
      }

      ImGui::SameLine();
      if (ImGui::CheckboxFlags("Limit pitch", static_cast<u32*>(&flags),
                               Camera::Masks::PitchLimit)) {
        camera_->flags() = flags;
      }

      auto angle = camera_->angle().target();
      if (ImGui::DragScalar("Angle", ImGuiDataType_Float,
                            reinterpret_cast<void*>(&angle), 0.1f, &limits.min,
                            &limits.max, "%+.2f")) {
        camera_->angle(angle);
      }

      if (ImGui::DragScalar("Y-Offset", ImGuiDataType_Float,
                            reinterpret_cast<void*>(&config.vertOff), 0.1f,
                            &limits.zero, &limits.max, "%+.2f"))
        updateConfig = true;

      if (ImGui::DragScalar("Speed factor", ImGuiDataType_Float,
                            reinterpret_cast<void*>(&config.spdFactor), 0.005f,
                            &limits.zero, &limits.max, "%+.2f"))
        updateConfig = true;

      if (ImGui::DragScalar("Speed factor min", ImGuiDataType_Float,
                            reinterpret_cast<void*>(&config.spdFactorMin),
                            0.005f, &limits.zero, &limits.max, "%+.2f"))
        updateConfig = true;

      static bool doSpin = false;
      static i32 doSpinRate = 15;
      ImGui::Checkbox("Spin", &doSpin);
      if (doSpin) {
        ImGui::SameLine();
        ImGui::SliderInt("Rate", &doSpinRate, limits.zero, limits.threeSixty);
        camera_->angle(camera_->angle().target() + doSpinRate * state.delta());
      }

      ImGui::Text("Speed scalar  %+.2f", camera_->translationFactor());

      const auto forward = getForward(camera_->pitch(), camera_->angle());
      const auto right = getRight(camera_->angle());
      const auto up = getUp(forward, right);
      ImGui::Text("Position      %+.2f %+.2f %+.2f", camera_->position().x,
                  camera_->position().y, camera_->position().z);
      ImGui::Text(
          "Target        %+.2f %+.2f %+.2f", camera_->target().x().target(),
          camera_->target().y().target(), camera_->target().z().target());
      ImGui::Text("Forward       %+.2f %+.2f %+.2f", forward.x, forward.y,
                  forward.z);
      ImGui::Text("Right         %+.2f %+.2f %+.2f", right.x, right.y, right.z);
      ImGui::Text("Up            %+.2f %+.2f %+.2f", up.x, up.y, up.z);

      ImGui::TreePop();
      ImGui::Separator();

      if (updateConfig) camera_->config(config);
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_UI_H_
