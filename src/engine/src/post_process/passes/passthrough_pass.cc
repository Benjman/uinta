#include "uinta/post_process/passes/passthrough_pass.h"

#include "uinta/post_process/blit_helper.h"

namespace uinta {

void PassthroughPass::execute(const PassContext& ctx) noexcept {
  if (ctx.blit == nullptr) {
    return;
  }
  ctx.blit->draw(ctx.input, ctx.output, ctx.width, ctx.height);
}

}  // namespace uinta
