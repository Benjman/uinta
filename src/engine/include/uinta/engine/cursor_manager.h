#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_uint2.hpp"
#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_manager.h"
#include "uinta/component.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_state.h"
#include "uinta/math/spatial.h"
#include "uinta/window.h"

namespace uinta {

class CursorManager final : public PreRenderComponent {
 public:
  CursorManager(Scene* scene) noexcept
      : window_(scene->engine()->platform()->window()),
        input_(scene->engine()->input()) {
    if (auto opt = scene->findSystem<CameraManager>(); opt.has_value()) {
      auto* cm = opt.value();
      camera_ = cm->camera();
      cm->addListener<CameraEvent::ViewMatrixUpdated>(
          [this](const auto&) { updateCursorWorldPoint(); });
      cm->addListener<CameraEvent::ProjectionMatrixUpdated>(
          [this](const auto& event) {
            projection_ = event.projection;
            updateCursorWorldPoint();
          });
    }
  }

  void updateCursorWorldPoint() noexcept {
    glm::vec2 viewport(window_->width(), window_->height());
    glm::vec3 ndc((2 * cursorPos_.x) / viewport.x - 1,
                  1 - (2 * cursorPos_.y) / viewport.y, 1);
    auto view = camera_->viewMatrix();
    input_->cursorWorldPoint(getWorldPoint(
        viewport, cursorPos_, camera_->position(), view, projection_));
  }

  void update(time_t) noexcept override {
    if (input_->cursordx() || input_->cursordy()) {
      cursorPos_ = {input_->cursorx(), input_->cursory()};
      updateCursorWorldPoint();
    }
  }

  glm::uvec2 cursorPos_;
  glm::mat4 projection_;

  const Camera* camera_;
  const Window* window_;
  Input* input_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_
