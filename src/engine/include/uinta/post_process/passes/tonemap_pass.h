#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_TONEMAP_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_TONEMAP_PASS_H_

#include "uinta/gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"

namespace uinta {

/*! Tonemapping pass: maps HDR scene-referred linear color into sRGB LDR.
 *
 *  Operators:
 *    - `AgX`:     fitted Sobotka AgX transform (stylized-friendly default).
 *    - `ACES`:    fitted filmic (Stephen Hill).
 *    - `Reinhard`: classic Reinhard-Jodie.
 *
 *  Typically installed as the last HDR-processing pass in the chain.
 */
class TonemapPass : public PostProcessPass {
 public:
  enum class Operator : u8 {
    AgX = 0,
    ACES = 1,
    Reinhard = 2,
  };

  explicit TonemapPass(const OpenGLApi* gl) noexcept;

  void execute(const PassContext& ctx) noexcept override;

  [[nodiscard]] std::string_view name() const noexcept override {
    return "Tonemap";
  }

  void onDebugUi() noexcept override;

  void op(Operator op) noexcept { op_ = op; }
  [[nodiscard]] Operator op() const noexcept { return op_; }

  void exposure(f32 exposure) noexcept { exposure_ = exposure; }
  [[nodiscard]] f32 exposure() const noexcept { return exposure_; }

 private:
  const OpenGLApi* gl_;
  Shader shader_;
  Uniform1i uInput_;
  Uniform1f uExposure_;
  Uniform1i uOperator_;

  Operator op_ = Operator::AgX;
  f32 exposure_ = 1.0f;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_TONEMAP_PASS_H_
