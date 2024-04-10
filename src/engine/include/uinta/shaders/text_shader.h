#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_SHADER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_SHADER_H_

#include "uinta/font.h"
#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class TextShader : public Shader {
  static constexpr auto FragmentShaderPath = "shader/text.fs.glsl";
  static constexpr auto VertexShaderPath = "shader/text.vs.glsl";

 public:
  Uniform1i atlas = {"uAtlas", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv transform = {"uTransform", this};

  explicit TextShader(const OpenGLApi* gl) noexcept
      : Shader(
            {
                {GL_VERTEX_SHADER, VertexShaderPath},
                {GL_FRAGMENT_SHADER, FragmentShaderPath},
            },
            gl) {
    ShaderGuard guard(this);
    atlas = 0;
    projection = glm::mat4(1);
    transform = glm::mat4(1);
  }

  void linkAttributes(const Vao* vao) const noexcept {
    constexpr Attribute position(0, 3, GL_FLOAT, GL_FALSE,
                                 ElementsPerTextVertex * sizeof(GLfloat),
                                 0 * sizeof(GLfloat));
    constexpr Attribute color(1, 4, GL_FLOAT, GL_FALSE,
                              ElementsPerTextVertex * sizeof(GLfloat),
                              3 * sizeof(GLfloat));
    constexpr Attribute uv(2, 2, GL_FLOAT, GL_FALSE,
                           ElementsPerTextVertex * sizeof(GLfloat),
                           7 * sizeof(GLfloat));
    vao->linkAttribute(position);
    vao->linkAttribute(color);
    vao->linkAttribute(uv);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_SHADER_H_
