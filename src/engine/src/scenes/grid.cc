#include "uinta/scenes/grid.h"

#include "uinta/engine/engine.h"
#include "uinta/engine/engine_state.h"
#include "uinta/gl.h"

namespace uinta {

GridScene::GridScene(Scene* parent, Engine* engine, size_t size,
                     const Camera* camera) noexcept
    : Scene(parent, engine, Layer::Debug),
      shader_(engine->gl()),
      vbo_(GL_ARRAY_BUFFER, 0, engine->gl()),
      color_(0.15f),
      camera_(camera),
      gl_(engine->gl()) {
  const auto half = size * 0.5;

  std::vector<glm::vec2> vertices;
  vertices.reserve(4 * (size + 1));

  for (size_t i = 0; i < size; i++) {
    vertices.push_back({0 - half, i - half});
    vertices.push_back({size - half, i - half});
    vertices.push_back({i - half, 0 - half});
    vertices.push_back({i - half, size - half});
  }

  vertices.push_back({0 - half, size - half});
  vertices.push_back({size - half, size - half});
  vertices.push_back({size - half, 0 - half});
  vertices.push_back({size - half, size - half});

  VaoGuard vag(&vao_);
  VboGuard vbg(&vbo_);
  vbo_.bufferData(vertices.data(), vertices.size() * sizeof(glm::vec2),
                  GL_STATIC_DRAW);

  shader_.linkAttributes(&vao_);

  renderCount_ = vertices.size();

  ShaderGuard sg(&shader_);
  shader_.color = color_;

  this->engine()->addListener<EngineEvent::ViewportSizeChange>(
      [&](const auto& event) {
        ShaderGuard sg(&shader_);
        shader_.projection = camera_->perspectiveMatrix(event.aspect());
      });
}

void GridScene::render(const EngineState&) noexcept {
  DepthTestGuard dtg(flags_.isDepthTest(), gl_);
  ShaderGuard sg(&shader_);

  if (comparator_ != camera_) {
    comparator_ = *camera_;
    shader_.modelView = camera_->viewMatrix();
  }

  if (flags_.isColorDirty()) {
    shader_.color = color_;
    flags_.isColorDirty(false);
  }

  VaoGuard vg(&vao_);
  gl_->drawArrays(GL_LINES, 0, renderCount_);
}

}  // namespace uinta
