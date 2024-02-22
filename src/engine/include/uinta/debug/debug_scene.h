#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_

#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/shaders/primitive.h"

namespace uinta {

class DebugScene : public Scene {
 public:
  struct Params final {
    Engine* engine;
    PrimitiveShader* shader;
    const OpenGLApi* gl;

    Params(Engine* engine, const OpenGLApi* gl) noexcept
        : engine(engine), gl(gl) {}
  };

  explicit DebugScene(Params, Layer) noexcept;

  const Params& params() const noexcept { return params_; }

  void onViewportSizeChange(const ViewportSizeChange&) noexcept override;
  void render(const EngineState& state) noexcept override;
  PrimitiveShader* shader() noexcept { return &shader_; }

 protected:
  PrimitiveShader shader_;
  const OpenGLApi* gl_;
  Params params_;
};

using DebugSceneParams = DebugScene::Params;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_
