#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_DEPTH_VISUALIZER_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_DEPTH_VISUALIZER_PASS_H_

#include "uinta/gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

/*! Debug pass. Samples `ctx.sceneTarget->depth()`, linearizes it, and writes
 *  it as grayscale. Ignores `ctx.input`. Disabled by default.
 */
class DepthVisualizerPass : public PostProcessPass {
 public:
  explicit DepthVisualizerPass(const OpenGLApi* gl) noexcept;

  void execute(const PassContext& ctx) noexcept override;

  [[nodiscard]] std::string_view name() const noexcept override {
    return "DepthVisualizer";
  }

  void onDebugUi() noexcept override;

 private:
  const OpenGLApi* gl_;
  Shader shader_;
  Uniform1i uDepth_;
  Uniform1f uNear_;
  Uniform1f uFar_;
  f32 lastNear_ = 0;
  f32 lastFar_ = 0;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_DEPTH_VISUALIZER_PASS_H_
