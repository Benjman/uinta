#include "uinta/scenes/axis_viewer.h"

#include <array>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/camera/camera.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/shader.h"

namespace uinta {

AxisViewerScene::AxisViewerScene(Scene* parent, Engine* engine,
                                 const Camera* camera) noexcept
    : Scene(parent, engine, Layer::Debug),
      shader_(engine->gl()),
      camera_(camera),
      vbo_(GL_ARRAY_BUFFER, 0, engine->gl()),
      gl_(engine->gl()) {
  constexpr glm::vec3 o(0, 0, 0);
  constexpr glm::vec3 x(1, 0, 0);
  constexpr glm::vec3 y(0, 1, 0);
  constexpr glm::vec3 z(0, 0, 1);
  constexpr glm::vec3 r(1, 0, 0);
  constexpr glm::vec3 g(0, 1, 0);
  constexpr glm::vec3 b(0, 0, 1);
  constexpr std::array<glm::vec3, 12> vertices{o, r, x, r, o, g,
                                               y, g, o, b, z, b};
  const auto data = vertices.data();
  const auto size = vertices.size() * sizeof(glm::vec3);
  VaoGuard vag(&vao_);
  VboGuard vbg(&vbo_);
  vbo_.bufferData(data, size, GL_STATIC_DRAW);
  shader_.linkAttributes(&vao_);

  this->engine()->addListener<EngineEvent::ViewportSizeChange>(
      [&](const auto& event) {
        constexpr auto orthoLeft = -1.5f;
        constexpr auto orthoRight = 20.0f;
        constexpr auto orthoBottom = -1.5f;
        constexpr auto orthoTop = 20.0f;

        auto right = orthoRight;
        auto top = orthoTop;

        if (event.aspect() > 1)
          right = orthoLeft + (orthoTop - orthoBottom) * event.aspect();
        else
          top = orthoBottom + (orthoRight - orthoLeft) / event.aspect();

        ShaderGuard sg(&shader_);
        shader_.projection = glm::ortho(orthoLeft, right, orthoBottom, top);
      });
}

void AxisViewerScene::render(const EngineState&) noexcept {
  ShaderGuard sg(&shader_);
  VaoGuard vg(&vao_);

  if (comparator_ != camera_) {
    comparator_ = *camera_;
    auto view = camera_->viewMatrix();
    view[3] = {0, 0, 0, 1};  // Removes translation factor
    shader_.view = view;
  }

  gl_->drawArrays(GL_LINES, 0, 6);
}

}  // namespace uinta
