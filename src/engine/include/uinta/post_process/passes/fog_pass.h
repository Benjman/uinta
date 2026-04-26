#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_FOG_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_FOG_PASS_H_

#include <glm/vec3.hpp>

#include "uinta/gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

/*! Depth-based exponential fog. Reconstructs world-space position from the
 *  scene depth buffer and the inverse view-projection matrix in
 *  `PassContext` and applies `1 - exp(-distance * density)` in the scene's
 *  world units.
 *
 *  Run before Tonemap (writes HDR-valued fog color) so the composite
 *  respects the tonemap curve. Disabled by default so an unconfigured scene
 *  doesn't fog unexpectedly.
 */
class FogPass : public PostProcessPass {
 public:
  explicit FogPass(const OpenGLApi* gl) noexcept;

  void execute(const PassContext& ctx) noexcept override;

  [[nodiscard]] std::string_view name() const noexcept override {
    return "Fog";
  }

  void onDebugUi() noexcept override;

  void color(glm::vec3 c) noexcept { color_ = c; }
  [[nodiscard]] glm::vec3 color() const noexcept { return color_; }

  void density(f32 v) noexcept { density_ = v; }
  [[nodiscard]] f32 density() const noexcept { return density_; }

  /*! Fraction of the far clipping plane at which the smooth fade to fully
   *  fogged begins (e.g. 0.9 = fade over the last 10% of view distance).
   *  Values >= 1.0 disable the fade. */
  void farFadeStart(f32 v) noexcept { farFadeStart_ = v; }
  [[nodiscard]] f32 farFadeStart() const noexcept { return farFadeStart_; }

 private:
  const OpenGLApi* gl_;
  Shader shader_;
  Uniform1i uInput_;
  Uniform1i uDepth_;
  UniformMatrix4fv uInvViewProjection_;
  Uniform3f uCameraPos_;
  Uniform3f uFogColor_;
  Uniform1f uDensity_;
  Uniform1f uNear_;
  Uniform1f uFar_;
  Uniform1f uFarFadeStart_;

  glm::vec3 color_ = glm::vec3(0.62, 0.67, 0.75);
  f32 density_ = 0.005f;
  f32 farFadeStart_ = 0.9f;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_FOG_PASS_H_
