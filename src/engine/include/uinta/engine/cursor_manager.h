#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint2.hpp>

#include "uinta/camera/camera_manager.h"
#include "uinta/component.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_state.h"
#include "uinta/math/spatial.h"
#include "uinta/window.h"

namespace uinta {

class CursorManager final : public NewFrameComponent {
 public:
  CursorManager(Engine* engine, CameraManager* camera) noexcept
      : camera_(camera->camera()),
        window_(engine->platform()->window()),
        state_(&engine->state()) {
    camera->addListener<CameraEvent::ViewMatrixUpdated>(
        [&](const auto&) { updateCursorWorldPoint(); });
  }

  void updateCursorWorldPoint() noexcept {
    glm::vec2 viewport(window_->width(), window_->height());
    glm::vec3 ndc((2 * cursorPos_.x) / viewport.x - 1,
                  1 - (2 * cursorPos_.y) / viewport.y, 1);
    f32 aspect = viewport.x / viewport.y;
    auto view = camera_->viewMatrix();
    auto proj = camera_->perspectiveMatrix(aspect);
    state_->input().cursorWorldPoint(
        getWorldPoint(viewport, cursorPos_, camera_->position(), view, proj));
  }

  void update(const EngineState& state) noexcept override {
    if (state.input().cursordx() || state.input().cursordy()) {
      cursorPos_ = {state.input().cursorx(), state.input().cursory()};
      updateCursorWorldPoint();
    }
  }

  glm::uvec2 cursorPos_;

  const Camera* camera_;
  const Window* window_;
  EngineState* state_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_
