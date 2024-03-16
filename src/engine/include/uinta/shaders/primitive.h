#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_

#include "uinta/primitive.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class PrimitiveShader : public Shader {
 public:
  // NOTE: These values must match those in the GLSL primitive shaders.
  static constexpr bitflag UvMask = 1 << 1;
  static constexpr bitflag SwayMask = 1 << 0;

  explicit PrimitiveShader(
      const OpenGLApi* gl = OpenGLApiImpl::GetInstance()) noexcept
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
    lightDir = glm::normalize(glm::vec3(-0.6, -1, -0.8));
    model = glm::mat4(1);
    projection = glm::mat4(1);
    view = glm::mat4(1);
    time = 0;
    flags = 0;
  }

  Uniform1f ambientStr = {"uAmbientStr", this};
  Uniform1f diffuseMin = {"uDiffuseMin", this};
  Uniform1f time = {"uTime", this};
  Uniform3f lightColor = {"uLightColor", this};
  Uniform3f lightDir = {"uLightDir", this};
  UniformMatrix4fv model = {"uModel", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv view = {"uView", this};

  void sway(bool enable = true) noexcept { setFlag(enable, SwayMask); }
  void uv(bool enable = true) noexcept { setFlag(enable, UvMask); }

  void linkAttributes(const Vao* vao) const noexcept {
    constexpr Attribute position(0, 3, GL_FLOAT, GL_FALSE,
                                 Vertex::ElementCount * sizeof(GLfloat),
                                 0 * sizeof(GLfloat));
    constexpr Attribute normal(1, 3, GL_FLOAT, GL_FALSE,
                               Vertex::ElementCount * sizeof(GLfloat),
                               3 * sizeof(GLfloat));
    constexpr Attribute color(2, 3, GL_FLOAT, GL_FALSE,
                              Vertex::ElementCount * sizeof(GLfloat),
                              6 * sizeof(GLfloat));
    constexpr Attribute uv(3, 2, GL_FLOAT, GL_FALSE,
                           Vertex::ElementCount * sizeof(GLfloat),
                           9 * sizeof(GLfloat));
    vao->linkAttribute(position);
    vao->linkAttribute(normal);
    vao->linkAttribute(color);
    vao->linkAttribute(uv);
  }

 private:
  void setFlag(bool enable, bitflag8 mask) noexcept {
    bool isEnabled = flags.value() & mask;
    if (isEnabled && !enable) {
      flags = (flags.value() & ~mask);
    } else if (!isEnabled && enable) {
      flags = (flags.value() | mask);
    }
  }

  Uniform1i flags = {"uFlags", this};
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
