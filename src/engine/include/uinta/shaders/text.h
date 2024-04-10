#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_H_

#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class TextShader : public Shader {
 public:
  explicit TextShader(
      const OpenGLApi* gl = OpenGLApiImpl::GetInstance()) noexcept
      : Shader(
            {
                {GL_VERTEX_SHADER, "text.vs.glsl"},
                {GL_FRAGMENT_SHADER, "text.fs.glsl"},
            },
            gl) {
    ShaderGuard guard(this);
    alpha = 1;
    atlas = 0;
    projection = glm::mat4(1);
    transform = glm::mat4(1);
  }

  Uniform1f alpha = {"uAlpha", this};
  Uniform1i atlas = {"uAtlas", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv transform = {"uTransform", this};

  void linkAttributes(const Vao* vao) const noexcept {
    constexpr Attribute position(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                                 0 * sizeof(GLfloat));
    constexpr Attribute color(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              3 * sizeof(GLfloat));
    constexpr Attribute uv(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                           6 * sizeof(GLfloat));
    vao->linkAttribute(position);
    vao->linkAttribute(color);
    vao->linkAttribute(uv);
  }

 private:
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_H_
