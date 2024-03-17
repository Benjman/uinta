#include "uinta/shaders/primitive.h"

namespace uinta {

PrimitiveShader::PrimitiveShader(const OpenGLApi* gl) noexcept
    : Shader(
          {
              {GL_VERTEX_SHADER, "primitive.vs.glsl"},
              {GL_FRAGMENT_SHADER, "primitive.fs.glsl"},
          },
          gl) {
  ShaderGuard guard(this);
  ambientStr = .15;
  diffuseMin = .125;
  lightColor = glm::vec3(1);
  lightDir = glm::normalize(glm::vec3(-0.25, -1, -0.8));
  model = glm::mat4(1);
  projection = glm::mat4(1);
  view = glm::mat4(1);
  time = 0;
  sway = true;
}

}  // namespace uinta
