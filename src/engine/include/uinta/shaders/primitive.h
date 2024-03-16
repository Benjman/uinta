#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_

#include "uinta/mesh.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class PrimitiveShader : public Shader {
  using PrimitiveShaderFlags = u8;

 public:
  // NOTE: These values must match those in the GLSL primitive shaders.
  static constexpr PrimitiveShaderFlags SwayMask = 1 << 0;
  static constexpr PrimitiveShaderFlags UvMask = 1 << 1;

  UniformMatrix4fv model = {"uModel", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv view = {"uView", this};
  Uniform3f lightColor = {"uLightColor", this};
  Uniform3f lightDir = {"uLightDir", this};
  Uniform1f ambientStr = {"uAmbientStr", this};
  Uniform1f diffuseMin = {"uDiffuseMin", this};
  Uniform1f time = {"uTime", this};

  explicit PrimitiveShader(
      const OpenGLApi* gl = OpenGLApiImpl::Instance()) noexcept
      : Shader(
            {
                {GL_VERTEX_SHADER, "shader/primitive.vs.glsl"},
                {GL_FRAGMENT_SHADER, "shader/primitive.fs.glsl"},
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
  Uniform1i flags = {"uFlags", this};

  void setFlag(bool enable, PrimitiveShaderFlags mask) noexcept {
    bool isEnabled = flags.value() & mask;
    if (isEnabled && !enable) {
      flags = (flags.value() & ~mask);
    } else if (!isEnabled && enable) {
      flags = (flags.value() | mask);
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
