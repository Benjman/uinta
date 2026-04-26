#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_OUTLINE_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_OUTLINE_PASS_H_

#include <glm/vec3.hpp>

#include "uinta/gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

/*! Cel/toon outline pass. Draws silhouette and crease lines by detecting
 *  high local variance in linearized depth and in view-space normals
 *  reconstructed from depth. No G-buffer required.
 *
 *  Runs best after ColorGrading so outlines are preserved through tonemapping
 * and grading but still get AA'd.
 */
class OutlinePass : public PostProcessPass {
 public:
  explicit OutlinePass(const OpenGLApi* gl) noexcept;

  void execute(const PassContext& ctx) noexcept override;

  [[nodiscard]] std::string_view name() const noexcept override {
    return "Outline";
  }

  void onDebugUi() noexcept override;

  void color(glm::vec3 c) noexcept { color_ = c; }
  [[nodiscard]] glm::vec3 color() const noexcept { return color_; }

  void thickness(f32 v) noexcept { thickness_ = v; }
  [[nodiscard]] f32 thickness() const noexcept { return thickness_; }

  void depthSensitivity(f32 v) noexcept { depthSensitivity_ = v; }
  [[nodiscard]] f32 depthSensitivity() const noexcept {
    return depthSensitivity_;
  }

  void normalSensitivity(f32 v) noexcept { normalSensitivity_ = v; }
  [[nodiscard]] f32 normalSensitivity() const noexcept {
    return normalSensitivity_;
  }

  /*! Fraction of the far clipping plane at which the smooth fade-out of
   *  the outline begins (e.g. 0.9 = fade over the last 10% of view
   *  distance). Values >= 1.0 disable the fade. */
  void farFadeStart(f32 v) noexcept { farFadeStart_ = v; }
  [[nodiscard]] f32 farFadeStart() const noexcept { return farFadeStart_; }

 private:
  const OpenGLApi* gl_;
  Shader shader_;
  Uniform1i uInput_;
  Uniform1i uDepth_;
  UniformMatrix4fv uInvProjection_;
  Uniform2f uTexelSize_;
  Uniform3f uOutlineColor_;
  Uniform1f uThickness_;
  Uniform1f uDepthSensitivity_;
  Uniform1f uNormalSensitivity_;
  Uniform1f uNear_;
  Uniform1f uFar_;
  Uniform1f uFarFadeStart_;

  glm::vec3 color_ = glm::vec3(0.16, 0.16, 0.16);
  f32 thickness_ = 1.0;
  f32 depthSensitivity_ = 1.5;
  f32 normalSensitivity_ = 0.3;
  f32 farFadeStart_ = 0.9f;  // last 10% of view distance fades the outline out
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_OUTLINE_PASS_H_
