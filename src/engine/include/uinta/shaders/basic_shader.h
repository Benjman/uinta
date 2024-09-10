#ifndef SRC_ENGINE_INCLUDE_UINTA_SHADERS_BASIC_SHADER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SHADERS_BASIC_SHADER_H_

#include "uinta/flags.h"
#include "uinta/mesh.h"
#include "uinta/shader.h"
#include "uinta/system.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class Scene;

class BasicShader : public Shader {
  friend class BasicShaderManager;

 public:
  explicit BasicShader(const OpenGLApi* gl) noexcept
      : Shader(
            {
                {GL_VERTEX_SHADER, "shader/basic.vs.glsl"},
                {GL_FRAGMENT_SHADER, "shader/basic.fs.glsl"},
            },
            gl) {
    ShaderGuard guard(this);
    ambientStr = .15;
    diffuseMin = .125;
    lightColor = glm::vec3(1);
    lightDir = glm::normalize(glm::vec3(-0.8, -1, -0.5));
    model = glm::mat4(1);
    projection = glm::mat4(1);
    view = glm::mat4(1);
    time = 0;
    flags = 0;
  }

  void linkAttributes(const Vao* vao) const noexcept override {
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
  Uniform1f ambientStr = {"uAmbientStr", this};
  Uniform1f diffuseMin = {"uDiffuseMin", this};
  Uniform1f time = {"uTime", this};
  Uniform1i flags = {"uFlags", this};
  Uniform3f lightColor = {"uLightColor", this};
  Uniform3f lightDir = {"uLightDir", this};
  UniformMatrix4fv model = {"uModel", this};
  UniformMatrix4fv projection = {"uProjection", this};
  UniformMatrix4fv view = {"uView", this};
};

class BasicShaderManager : public System {
 public:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(DefaultFlags);

    bool isViewDirty() const noexcept { return flags_ & ViewDirtyMask; }
    void isViewDirty(bool v) noexcept {
      flags_ &= ~ViewDirtyMask;
      if (v) flags_ |= ViewDirtyMask;
    }

    bool isProjectionDirty() const noexcept {
      return flags_ & ProjectionDirtyMask;
    }
    void isProjectionDirty(bool v) noexcept {
      flags_ &= ~ProjectionDirtyMask;
      if (v) flags_ |= ProjectionDirtyMask;
    }

   private:
    static constexpr value_type ViewDirtyMask = 1 << 0;
    static constexpr value_type ProjectionDirtyMask = 1 << 1;

    static constexpr value_type DefaultFlags =
        ViewDirtyMask | ProjectionDirtyMask;

    value_type flags_;
  };

  explicit BasicShaderManager(Scene*) noexcept;

  void onPreRender(time_t delta) noexcept override {
    runtime_ += delta;
    renderCount_ = 0;

    ShaderGuard guard(&shader_);
    shader_.time = runtime_;
    if (flags_.isProjectionDirty()) {
      shader_.projection = projection_;
      flags_.isProjectionDirty(false);
    }
    if (flags_.isViewDirty()) {
      shader_.view = view_;
      flags_.isViewDirty(false);
    }
  }

  void drawElements(GLenum mode, GLsizei count, GLenum type,
                    const void* indices) noexcept {
    shader_.gl()->drawElements(mode, count, type, indices);
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
    shader_.gl()->drawArrays(mode, first, count);
    renderCount_ += count;
  }

  BasicShader* shader() noexcept { return &shader_; }

  const BasicShader* shader() const noexcept { return &shader_; }

  count_t renderCount() const noexcept { return renderCount_; }

  void renderCount(count_t count) noexcept { renderCount_ = count; }

  void ambientStr(f32 ambientStr) noexcept { shader_.ambientStr = ambientStr; }
  f32 ambientStr() const noexcept { return shader_.ambientStr.value(); }

  void diffuseMin(f32 diffuseMin) noexcept { shader_.diffuseMin = diffuseMin; }
  f32 diffuseMin() const noexcept { return shader_.diffuseMin.value(); }

  void time(f32 time) noexcept { shader_.time = time; }
  f32 time() const noexcept { return shader_.time.value(); }

  void flags(f32 flags) noexcept { shader_.flags = flags; }
  f32 flags() const noexcept { return shader_.flags.value(); }

  void lightColor(glm::vec3 lightColor) noexcept {
    shader_.lightColor = lightColor;
  }
  glm::vec3 lightColor() const noexcept { return shader_.lightColor.value(); }

  void lightDir(glm::vec3 lightDir) noexcept { shader_.lightDir = lightDir; }
  glm::vec3 lightDir() const noexcept { return shader_.lightDir.value(); }

  void model(glm::mat4 model) noexcept { shader_.model = model; }
  glm::mat4 model() const noexcept { return shader_.model.value(); }

  void projection(glm::mat4 projection) noexcept {
    shader_.projection = projection;
  }
  glm::mat4 projection() const noexcept { return shader_.projection.value(); }

  void view(glm::mat4 view) noexcept { shader_.view = view; }
  glm::mat4 view() const noexcept { return shader_.view.value(); }

  void linkAttributes(const Vao* vao) const noexcept {
    shader_.linkAttributes(vao);
  }

  // NOTE: These masks must match those in the basic GLSL shaders.
  struct Masks final {
    static constexpr u8 Sway = 1 << 0;
    static constexpr u8 Uv = 1 << 1;
    static constexpr u8 Normals = 1 << 2;
  };

  void sway(bool enable = true) noexcept { setFlag(enable, Masks::Sway); }
  void uv(bool enable = true) noexcept { setFlag(enable, Masks::Uv); }
  void normals(bool enable = true) noexcept { setFlag(enable, Masks::Normals); }

 private:
  glm::mat4 projection_;
  glm::mat4 view_;
  Flags flags_;
  size_t renderCount_ = 0;
  time_t runtime_ = 0;

  BasicShader shader_;

  void setFlag(bool enable, u8 mask) noexcept {
    bool isEnabled = shader_.flags.value() & mask;
    if (isEnabled && !enable) {
      shader_.flags = (shader_.flags.value() & ~mask);
    } else if (!isEnabled && enable) {
      shader_.flags = (shader_.flags.value() | mask);
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SHADERS_BASIC_SHADER_H_
