#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_

#include "uinta/engine/engine_events.h"
#include "uinta/mesh.h"
#include "uinta/shader.h"
#include "uinta/system.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class PrimitiveShader : public Shader {
 public:
  UniformMatrix4fv model = {"uModel", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv view = {"uView", this};
  Uniform3f lightColor = {"uLightColor", this};
  Uniform3f lightDir = {"uLightDir", this};
  Uniform1f ambientStr = {"uAmbientStr", this};
  Uniform1f diffuseMin = {"uDiffuseMin", this};
  Uniform1f time = {"uTime", this};

  explicit PrimitiveShader(const OpenGLApi* gl) noexcept
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

  // NOTE: These masks must match those in the primitive GLSL shaders.
  struct Masks final {
    static constexpr u8 Sway = 1 << 0;
    static constexpr u8 Uv = 1 << 1;
    static constexpr u8 Normals = 1 << 2;
  };

  void sway(bool enable = true) noexcept { setFlag(enable, Masks::Sway); }
  void uv(bool enable = true) noexcept { setFlag(enable, Masks::Uv); }
  void normals(bool enable = true) noexcept { setFlag(enable, Masks::Normals); }

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

  void renderNormals() noexcept {}

  count_t renderCount() const noexcept { return renderCount_; }

  void renderCount(count_t count) noexcept { renderCount_ = count; }

  void drawElements(GLenum mode, GLsizei count, GLenum type,
                    const void* indices) noexcept {
    gl()->drawElements(mode, count, type, indices);
    switch (mode) {
      case GL_POINTS:
        renderCount_ += count;
        break;
      case GL_LINE_STRIP:
        renderCount_ += (count - 1);
        break;
      case GL_LINE_LOOP:
        renderCount_ += count;
        break;
      case GL_LINES:
        renderCount_ += count / 2;
        break;
      case GL_LINE_STRIP_ADJACENCY:
        renderCount_ += (count - 3);
        break;
      case GL_LINES_ADJACENCY:
        renderCount_ += count / 4;
        break;
      case GL_TRIANGLE_STRIP:
        renderCount_ += (count - 2);
        break;
      case GL_TRIANGLE_FAN:
        renderCount_ += (count - 2);
        break;
      case GL_TRIANGLES:
        renderCount_ += count / 3;
        break;
      case GL_TRIANGLE_STRIP_ADJACENCY:
        renderCount_ += (count - 4);
        break;
      case GL_TRIANGLES_ADJACENCY:
        renderCount_ += count / 6;
        break;
    }
  }

  void drawArrays(GLenum mode, GLint first, GLsizei count) noexcept {
    gl()->drawArrays(mode, first, count);
    renderCount_ += count;
  }

 private:
  Uniform1i flags = {"uFlags", this};

  void setFlag(bool enable, u8 mask) noexcept {
    bool isEnabled = flags.value() & mask;
    if (isEnabled && !enable) {
      flags = (flags.value() & ~mask);
    } else if (!isEnabled && enable) {
      flags = (flags.value() | mask);
    }
  }

  count_t renderCount_ = 0;
};

class PrimitiveShaderManager : public System {
 public:
  explicit PrimitiveShaderManager(PrimitiveShader* shader,
                                  EngineDispatchers* engine) noexcept
      : shader_(shader) {
    engine->addListener<EngineEvent::ViewportSizeChange>(
        [this](const auto& event) {
          ShaderGuard guard(shader_);
          shader_->projection = glm::perspective(
              glm::radians(45.0), static_cast<f64>(event.aspect()), 0.01, 2.0);
        });
  }

  void onNewFrame(const EngineState& state) noexcept override {
    ShaderGuard sg(shader_);
    shader_->time = state.runtime();
    shader_->renderCount(0);
  }

 private:
  PrimitiveShader* shader_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_PRIMITIVE_H_
