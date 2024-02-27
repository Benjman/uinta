#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_

#include "uinta/engine_signal.h"
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

  void render(const EngineState&, const Input&) noexcept override;

  void onViewportSizeChange(const ViewportSizeChange&) noexcept override;

 private:
  struct AxisShader : Shader {
    AxisShader() noexcept;

    UniformMatrix4fv view = {"view", this};
    UniformMatrix4fv projection = {"projection", this};
  } shader_;
  const Camera* camera_;
  Vao vao_;
  Vbo vbo_ = GL_ARRAY_BUFFER;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_AXIS_VIEWER_H_
