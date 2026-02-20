#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_

#include "uinta/gl.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class CameraManager;

class AxisViewerScene : public Scene {
 public:
  AxisViewerScene(Scene*) noexcept;

  void render(time_t) noexcept override;

 private:
  struct AxisShader : Shader {
    UniformMatrix4fv view = {"uView", this};
    UniformMatrix4fv projection = {"uProjection", this};

    explicit AxisShader(const OpenGLApi* gl) noexcept
        : Shader(
              {
                  {GL_VERTEX_SHADER, "shader/axis.vs.glsl"},
                  {GL_FRAGMENT_SHADER, "shader/axis.fs.glsl"},
              },
              gl) {}

    void linkAttributes(const Vao* vao) const noexcept {
      constexpr Attribute position(0, 3, GL_FLOAT, GL_FALSE,
                                   6 * sizeof(GLfloat), 0);
      constexpr Attribute color(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                3 * sizeof(GLfloat));
      vao->linkAttribute(position);
      vao->linkAttribute(color);
    }
  } shader_;
  struct Flags final {
    using value_type = u8;

    FlagsOperations(ProjectionDirtyMask | ViewDirtyMask);

    bool isProjectionDirty() const noexcept {
      return flags_ & ProjectionDirtyMask;
    }
    void isProjectionDirty(bool v) noexcept {
      flags_ &= ~ProjectionDirtyMask;
      if (v) flags_ |= ProjectionDirtyMask;
    }

    bool isViewDirty() const noexcept { return flags_ & ViewDirtyMask; }
    void isViewDirty(bool v) noexcept {
      flags_ &= ~ViewDirtyMask;
      if (v) flags_ |= ViewDirtyMask;
    }

   private:
    static constexpr value_type ProjectionDirtyMask = 1 << 0;
    static constexpr value_type ViewDirtyMask = 1 << 1;

    value_type flags_;
  } flags_;
  Vao vao_;
  Vbo vbo_;
  glm::mat4 view_, projection_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_
