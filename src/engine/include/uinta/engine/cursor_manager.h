#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint2.hpp>

#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_manager.h"
#include "uinta/component.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/service_handle.h"
#include "uinta/math/spatial.h"
#include "uinta/viewport/viewport_manager.h"
#include "uinta/window.h"

namespace uinta {

class CursorManager final : public PreRenderComponent {
 public:
  CursorManager(Engine* engine) noexcept
      : serviceHandle_(engine, this),
        window_(engine->platform()->window()),
        input_(engine->input()) {
    auto* cameraManager = engine->service<CameraManager>();
    assert(cameraManager);
    camera_ = cameraManager->camera();
    cameraManager->addListener<CameraEvent::ViewMatrixUpdated>(
        [this](const auto&) { updateCursorWorldPoint(); });
    auto* viewportManager = engine->service<ViewportManager>();
    assert(viewportManager);
    viewportManager->addListener<ViewportEvent::ProjectionMatrixUpdated>(
        [this](const auto& event) {
          projection_ = event.projection;
          updateCursorWorldPoint();
        });
  }

  void updateCursorWorldPoint() const noexcept {
    glm::vec2 viewport(window_->width(), window_->height());
    glm::vec3 ndc(((static_cast<f32>(2 * cursorPos_.x)) / viewport.x) - 1,
                  static_cast<f32>(1 - (2 * cursorPos_.y)) / viewport.y, 1.0f);
    auto view = camera_->viewMatrix();
    input_->cursorWorldPoint(getWorldPoint(
        viewport, cursorPos_, camera_->position(), view, projection_));
  }

  void update(time_t /*unused*/) noexcept override {
    if ((input_->cursordx() != 0.0f) || (input_->cursordy() != 0.0f)) {
      cursorPos_ = {input_->cursorx(), input_->cursory()};
      updateCursorWorldPoint();
    }
  }

  glm::uvec2 cursorPos_;
  glm::mat4 projection_;

 private:
  ServiceHandle<CursorManager> serviceHandle_;
  const Camera* camera_ = nullptr;
  const Window* window_;
  Input* input_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_
