#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_PASSTHROUGH_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_PASSTHROUGH_PASS_H_

#include "uinta/gl.h"
#include "uinta/post_process/pass.h"

namespace uinta {

/*! Identity pass: samples `ctx.input` and writes it to `ctx.output`. Useful as
 *  a debugging/verification step and as a template for new passes.
 *
 *  Implemented by delegating to the shared `BlitHelper`, so there is no
 *  duplicate shader.
 */
class PassthroughPass : public PostProcessPass {
 public:
  explicit PassthroughPass(const OpenGLApi* gl) noexcept : gl_(gl) {}

  void execute(const PassContext& ctx) noexcept override;

  [[nodiscard]] std::string_view name() const noexcept override {
    return "Passthrough";
  }

 private:
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASSES_PASSTHROUGH_PASS_H_
