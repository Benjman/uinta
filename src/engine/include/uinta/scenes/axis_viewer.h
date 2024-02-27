#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_

#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Camera;

class AxisViewer : public Scene {
 public:
  AxisViewer(const Camera*,
             const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  void render(const EngineState&) noexcept override;

  void onViewportSizeChange(const ViewportSizeChange&) noexcept override;

 private:
  struct AxisShader : Shader {
    UniformMatrix4fv view = {"uView", this};
    UniformMatrix4fv projection = {"uProjection", this};

    AxisShader() noexcept
        : Shader({
              {GL_VERTEX_SHADER, "axis.vs.glsl"},
              {GL_FRAGMENT_SHADER, "axis.fs.glsl"},
          }) {}

    void linkAttributes(const Vao* vao) const noexcept {
      constexpr Attribute position(0, 3, GL_FLOAT, GL_FALSE,
                                   6 * sizeof(GLfloat), 0);
      constexpr Attribute color(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                3 * sizeof(GLfloat));
      vao->linkAttribute(position);
      vao->linkAttribute(color);
    }
  } shader_;
  const Camera* camera_;
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_
