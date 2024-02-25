#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_

#include "uinta/camera/comparator.h"
#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/types.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Camera;

class GridScene : public Scene {
  using GridSceneFlags = u8;

 public:
  explicit GridScene(Scene* parent, size_t size, const Camera*,
                     const OpenGLApi* = OpenGLApiImpl::Instance()) noexcept;

  void tick(const EngineState&) noexcept override;

  void render(const EngineState&) noexcept override;

  void onViewportSizeChange(const ViewportSizeChange&) noexcept override;

  void depthTesting(bool v) noexcept {
    flags_ &= ~DepthTestMask;
    if (v) flags_ |= DepthTestMask;
  }

  bool depthTesting() const noexcept { return flags_ & DepthTestMask; }

  void color(const glm::vec4& color) {
    color_ = color;
    flags_ |= ColorDirtyMask;
  }

 private:
  static constexpr GridSceneFlags DepthTestMask = 1 << 0;
  static constexpr GridSceneFlags ColorDirtyMask = 1 << 1;

  struct GridShader : public Shader {
    UniformMatrix4fv modelView = {"uModelView", this};
    UniformMatrix4fv projection = {"uProjection", this};
    Uniform4f color = {"uColor", this};

    explicit GridShader(
        const OpenGLApi* gl = OpenGLApiImpl::Instance()) noexcept
        : Shader(
              {
                  {GL_VERTEX_SHADER, "shader/grid.vs.glsl"},
                  {GL_FRAGMENT_SHADER, "shader/grid.fs.glsl"},
              },
              gl) {}

    void linkAttributes(const Vao* vao) const noexcept {
      constexpr Attribute PositionAttrib(0, 2, GL_FLOAT, GL_FALSE,
                                         2 * sizeof(GLfloat), 0);
      vao->linkAttribute(PositionAttrib);
    }
  } shader_;
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
  glm::vec4 color_;
  const Camera* camera_;
  CameraComparator comparator_;
  const OpenGLApi* gl_;
  size_t renderCount_;
  GridSceneFlags flags_ = DepthTestMask;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_
