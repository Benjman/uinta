#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_COLOR_GRADING_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_COLOR_GRADING_PASS_H_

#include <memory>
#include <string>

#include "uinta/gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/uniform.h"

namespace uinta {

/*! 3D LUT color grading pass (LUT-strip format).
 *
 *  LUTs are authored externally (Photoshop / Davinci / GIMP) and exported
 *  as a `(N*N) x N` 2D strip PNG. This is the standard Unity/Unreal-style
 *  format. Typical sizes are N=16 (neutral 256x16) or N=32 (1024x32).
 *
 *  Without a loaded LUT the pass becomes a passthrough (intensity is
 *  implicitly 0 when `lutSize_ == 0`).
 *
 *  Install after Tonemap (operates on LDR sRGB input).
 */
class ColorGradingPass : public PostProcessPass {
 public:
  explicit ColorGradingPass(const OpenGLApi* gl) noexcept;

  void execute(const PassContext& ctx) noexcept override;

  [[nodiscard]] std::string_view name() const noexcept override {
    return "Color Grading";
  }

  void onDebugUi() noexcept override;

  /*! Load a LUT-strip PNG from disk. The file's width must equal
   *  `height * height`; `height` becomes the cube edge length. Returns
   *  `OkStatus()` on success. */
  Status setLut(const std::string& resourcePath) noexcept;

  /*! Release the current LUT and revert to passthrough. */
  void clearLut() noexcept;

  void intensity(f32 v) noexcept { intensity_ = v; }
  [[nodiscard]] f32 intensity() const noexcept { return intensity_; }

  [[nodiscard]] u32 lutSize() const noexcept { return lutSize_; }
  [[nodiscard]] const std::string& lutPath() const noexcept { return lutPath_; }

 private:
  const OpenGLApi* gl_;
  Shader shader_;
  Uniform1i uInput_;
  Uniform1i uLut_;
  Uniform1f uLutSize_;
  Uniform1f uIntensity_;

  std::unique_ptr<Texture> lut_;
  u32 lutSize_ = 0;
  std::string lutPath_;
  f32 intensity_ = 1.0f;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_COLOR_GRADING_PASS_H_
