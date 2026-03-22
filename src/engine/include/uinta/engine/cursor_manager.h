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
      : serviceHandle_(engine, this), window_(engine->platform()->window()), input_(engine->input()) {
    auto* cameraManager = engine->service<CameraManager>();
    assert(cameraManager);
    camera_ = cameraManager->camera();
    cameraManager->addListener<CameraEvent::ViewMatrixUpdated>([this](const auto&) { updateWorldPoint(); });
    auto* viewportManager = engine->service<ViewportManager>();
    assert(viewportManager);
    viewportManager->addListener<ViewportEvent::ProjectionMatrixUpdated>([this](const auto& event) {
      projection_ = event.projection;
      updateWorldPoint();
    });
  }

  [[nodiscard]] glm::vec3 worldPoint() const noexcept { return worldPoint_; }

  void update(time_t /*unused*/) noexcept override {
    if ((input_->cursordx() != 0.0f) || (input_->cursordy() != 0.0f)) {
      cursorPos_ = {input_->cursorx(), input_->cursory()};
      updateWorldPoint();
    }
  }

  glm::uvec2 cursorPos_;
  glm::mat4 projection_;

 private:
  ServiceHandle<CursorManager> serviceHandle_;
  const Camera* camera_ = nullptr;
  const Window* window_;
  Input* input_;
  glm::vec3 worldPoint_{0, 0, 0};

  void updateWorldPoint() noexcept {
    glm::vec2 viewport(window_->width(), window_->height());
    auto position = camera_->position();
    auto view = camera_->viewMatrix();
    worldPoint_ = getWorldPoint(viewport, cursorPos_, position, view, projection_);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_CURSOR_MANAGER_H_
