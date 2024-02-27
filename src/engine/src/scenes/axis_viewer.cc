#include "uinta/scenes/axis_viewer.h"

#include <array>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/camera/camera.h"
#include "uinta/shader.h"
#include "uinta/utils/viewport_change.h"

namespace uinta {

AxisViewerScene::AxisViewerScene(Scene* parent, const Camera* camera,
                                 const OpenGLApi* gl) noexcept
    : Scene(parent, Layer::Debug), shader_(gl), camera_(camera), gl_(gl) {
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

void AxisViewerScene::onViewportSizeChange(
    const ViewportSizeChange& event) noexcept {
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
}

}  // namespace uinta
