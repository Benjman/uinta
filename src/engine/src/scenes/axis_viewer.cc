#include "uinta/scenes/axis_viewer.h"

#include <array>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/camera/camera_events.h"
#include "uinta/camera/camera_manager.h"
#include "uinta/color.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/shader.h"

namespace uinta {

AxisViewerScene::AxisViewerScene(Scene* parent) noexcept
    : Scene(parent, SCENE_NAME, SceneLayer::Debug),
      shader_(engine()->gl()),
      vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
      gl_(engine()->gl()) {
  constexpr glm::vec3 o(0, 0, 0);
  constexpr glm::vec3 x(1, 0, 0);
  constexpr glm::vec3 y(0, 1, 0);
  constexpr glm::vec3 z(0, 0, 1);

  constexpr auto r = color::RedA200;
  constexpr auto g = color::GreenA200;
  constexpr auto b = color::BlueA200;

  constexpr std::array<glm::vec3, 12> vertices{o, r, x, r, o, g,
                                               y, g, o, b, z, b};
  const auto data = vertices.data();
  const auto size = vertices.size() * sizeof(glm::vec3);
  VaoGuard vag(&vao_);
  VboGuard vbg(&vbo_);
  vbo_.bufferData(data, size, GL_STATIC_DRAW);
  shader_.linkAttributes(&vao_);

  if (auto copt = parent->findSystem<CameraManager>(); copt.has_value()) {
    copt.value()->addListener<CameraEvent::ProjectionMatrixUpdated>(
        [this](const auto& event) {
          constexpr f32 orthoLeft = -1.5;
          constexpr f32 orthoRight = 20;
          constexpr f32 orthoBottom = -1.5;
          constexpr f32 orthoTop = 20;

          auto right = orthoRight;
          auto top = orthoTop;

          if (event.aspect > 1)
            right = orthoLeft + (orthoTop - orthoBottom) * event.aspect;
          else
            top = orthoBottom + (orthoRight - orthoLeft) / event.aspect;

          projection_ = glm::ortho(orthoLeft, right, orthoBottom, top);
          flags_.isProjectionDirty(true);
        });

    copt.value()->addListener<CameraEvent::ViewMatrixUpdated>(
        [this](const auto& event) {
          auto view = event.view;
          view[3] = {0, 0, 0, 1};  // Removes translation factor
          view_ = view;
          flags_.isViewDirty(true);
        });
  }
}

void AxisViewerScene::render(time_t) noexcept {
  ShaderGuard sg(&shader_);
  VaoGuard vg(&vao_);

  if (flags_.isProjectionDirty()) {
    shader_.projection = projection_;
    flags_.isProjectionDirty(false);
  }

  if (flags_.isViewDirty()) {
    shader_.view = view_;
    flags_.isViewDirty(false);
  }

  gl_->drawArrays(GL_LINES, 0, 6);
}

}  // namespace uinta
