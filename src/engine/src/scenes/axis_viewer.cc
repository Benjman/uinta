#include "uinta/scenes/axis_viewer.h"

#include <vector>

#include "absl/log/log.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float3.hpp"
#include "uinta/camera.h"
#include "uinta/engine_signal.h"
#include "uinta/engine_state.h"
#include "uinta/lib/absl/status.h"
#include "uinta/shader.h"

namespace uinta {

constexpr auto PositionOffset = 0 * sizeof(GLfloat);
constexpr auto ColorOffset = 3 * sizeof(GLfloat);
constexpr auto DataStride = 6 * sizeof(GLfloat);

AxisViewer::AxisViewer(const Camera* camera, const OpenGLApi* gl) noexcept
    : Scene(Layer::Debug), camera_(camera), gl_(gl) {
  glm::vec3 o(0, 0, 0);
  glm::vec3 x(1, 0, 0);
  glm::vec3 y(0, 1, 0);
  glm::vec3 z(0, 0, 1);

  glm::vec3 r(1, 0, 0);
  glm::vec3 g(0, 1, 0);
  glm::vec3 b(0, 0, 1);

  std::vector<glm::vec3> vertices{o, r, x, r, o, g, y, g, o, b, z, b};

  VaoGuard vg(&vao_);
  vbo_.bufferData(vertices.data(), vertices.size(), GL_STATIC_DRAW);
  vao_.linkAttribute({&vbo_, 0, 3, GL_FLOAT, GL_FALSE, DataStride,
                      reinterpret_cast<void*>(PositionOffset)});
  vao_.linkAttribute({&vbo_, 1, 3, GL_FLOAT, GL_FALSE, DataStride,
                      reinterpret_cast<void*>(ColorOffset)});
}

void AxisViewer::render(const EngineState&, const Input&) noexcept {
  glm::mat4 view = camera_->viewMatrix();

  // Remove translation factor:
  view[3] = {0, 0, 0, 1};

  ShaderGuard sg(&shader_);
  shader_.view = view;

  VaoGuard vg(&vao_);
  gl_->drawArrays(GL_LINES, 0, DataStride);
}

void AxisViewer::onViewportSizeChange(
    const ViewportSizeChange& event) noexcept {
  constexpr auto orthoLeft = -1.5f;
  constexpr auto orthoRight = 20.0f;
  constexpr auto orthoBottom = -1.5f;
  constexpr auto orthoTop = 20.0f;

  auto right = orthoRight;
  auto top = orthoTop;

  if (event.aspect() > 1) {
    right = orthoLeft + (orthoTop - orthoBottom) * event.aspect();
  } else {
    top = orthoBottom + (orthoRight - orthoLeft) / event.aspect();
  }

  ShaderGuard sg(&shader_);
  shader_.projection = glm::ortho(orthoLeft, right, orthoBottom, top);
}

AxisViewer::AxisShader::AxisShader() noexcept
    : Shader({
          {GL_VERTEX_SHADER, "axis.vs.glsl"},
          {GL_FRAGMENT_SHADER, "axis.fs.glsl"},
      }) {}

}  // namespace uinta
