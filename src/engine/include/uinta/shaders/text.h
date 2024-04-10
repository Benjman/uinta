#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_H_

#include "uinta/font.h"
#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class TextShader : public Shader {
 public:
  Uniform1i atlas = {"uAtlas", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv transform = {"uTransform", this};

  explicit TextShader(const OpenGLApi* gl) noexcept
      : Shader(
            {
                {GL_VERTEX_SHADER, "shader/text.vs.glsl"},
                {GL_FRAGMENT_SHADER, "shader/text.fs.glsl"},
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

  void draw(const Font* font, GLint first, GLsizei count) const noexcept {
    ShaderGuard guard(this);
    TextureGuard tg(&font->texture());
    ActiveTextureGuard atg(GL_TEXTURE0, gl());
    BlendGuard bg(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true, gl());
    gl()->drawArrays(GL_TRIANGLES, first, count);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_TEXT_H_
