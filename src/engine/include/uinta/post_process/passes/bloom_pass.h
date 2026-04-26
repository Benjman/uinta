#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_BLOOM_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_BLOOM_PASS_H_

#include "uinta/gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

/*! Energy-conserving dual-filter bloom (Call of Duty: Advanced Warfare 2014).
 *
 *  Pipeline:
 *    1. Progressive 13-tap downsample into an N-level mip chain. The first
 *       downsample uses a partial Karis average to kill fireflies.
 *    2. Progressive 9-tap tent upsample: upsample level k onto level k-1
 *       with additive blend.
 *    3. Composite: mix the top of the bloom mip chain into the scene color.
 *
 *  The intermediate mip chain is acquired transiently from the pool each
 *  frame. Default intensity is intentionally subtle; crank it up for a more
 *  stylized glow.
 */
class BloomPass : public PostProcessPass {
 public:
  static constexpr u32 kDefaultMipCount = 5;

  explicit BloomPass(const OpenGLApi* gl) noexcept;

  void execute(const PassContext& ctx) noexcept override;

  [[nodiscard]] std::string_view name() const noexcept override {
    return "Bloom";
  }

  void onDebugUi() noexcept override;

  void intensity(f32 v) noexcept { intensity_ = v; }
  [[nodiscard]] f32 intensity() const noexcept { return intensity_; }

  void threshold(f32 v) noexcept { threshold_ = v; }
  [[nodiscard]] f32 threshold() const noexcept { return threshold_; }

  void filterRadius(f32 v) noexcept { filterRadius_ = v; }
  [[nodiscard]] f32 filterRadius() const noexcept { return filterRadius_; }

  void mipCount(u32 v) noexcept { mipCount_ = v; }
  [[nodiscard]] u32 mipCount() const noexcept { return mipCount_; }

 private:
  const OpenGLApi* gl_;

  // Downsample shader.
  Shader downsampleShader_;
  Uniform1i dsInput_;
  Uniform2f dsTexelSize_;
  Uniform1i dsIsFirst_;

  // Upsample shader.
  Shader upsampleShader_;
  Uniform1i usInput_;
  Uniform1f usFilterRadius_;

  // Composite shader.
  Shader compositeShader_;
  Uniform1i compInput_;
  Uniform1i compBloom_;
  Uniform1f compIntensity_;
  Uniform1f compThreshold_;

  f32 intensity_ = 0.35f;
  f32 threshold_ = 0.0f;
  f32 filterRadius_ = 0.005f;
  u32 mipCount_ = kDefaultMipCount;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_BLOOM_PASS_H_
