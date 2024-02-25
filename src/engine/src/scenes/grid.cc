#include "uinta/scenes/grid.h"

#include "uinta/camera.h"
#include "uinta/engine_state.h"
#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/utils/viewport_change.h"

namespace uinta {

Grid::Grid(size_t size, const Camera* camera, const OpenGLApi* gl) noexcept
    : Scene(Layer::Debug),
      shader_(gl),
      color_(0.15f),
      camera_(camera),
      gl_(gl) {
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
}

void Grid::tick(const EngineState& state) noexcept {
  if (state.isKeyPressed(KEY_SPACE)) depthTesting(!depthTesting());
}

void Grid::render(const EngineState&) noexcept {
  DepthTestGuard dtg(flags_ & DepthTestMask);
  ShaderGuard sg(&shader_);
  shader_.modelView = camera_->viewMatrix();
  if (flags_ & ColorDirtyMask) {
    shader_.color = color_;
    flags_ &= ~ColorDirtyMask;
  }

  VaoGuard vg(&vao_);
  gl_->drawArrays(GL_LINES, 0, renderCount_);
}

void Grid::onViewportSizeChange(const ViewportSizeChange& event) noexcept {
  ShaderGuard sg(&shader_);
  shader_.projection = camera_->perspectiveMatrix(event.aspect());
}

}  // namespace uinta
