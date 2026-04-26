#include "uinta/post_process/passes/tonemap_pass.h"

#include <imgui.h>

#include "uinta/fbo.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

TonemapPass::TonemapPass(const OpenGLApi* gl) noexcept
    : gl_(gl),
      shader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER, "shader/post_process/tonemap.fs.glsl"},
          },
          gl),
      uInput_{"uInput", &shader_},
      uExposure_{"uExposure", &shader_},
      uOperator_{"uOperator", &shader_} {
  ShaderGuard guard(&shader_);
  uInput_ = 0;
  uExposure_ = exposure_;
  uOperator_ = static_cast<GLint>(op_);
  enabled(false);
}

void TonemapPass::execute(const PassContext& ctx) noexcept {
  if (ctx.input == nullptr) {
    return;
  }

  gl_->bindFramebuffer(GL_FRAMEBUFFER,
                       (ctx.output != nullptr) ? ctx.output->id() : 0);
  gl_->viewport(0, 0, static_cast<GLsizei>(ctx.width),
                static_cast<GLsizei>(ctx.height));

  ShaderGuard shaderGuard(&shader_);
  VaoGuard vaoGuard(ctx.fullscreenVao);

  uExposure_ = exposure_;
  uOperator_ = static_cast<GLint>(op_);

  ActiveTextureGuard activeGuard(GL_TEXTURE0, gl_);
  gl_->bindTexture(ctx.input->target(), ctx.input->id());

  gl_->drawArrays(GL_TRIANGLES, 0, 3);

  gl_->bindTexture(ctx.input->target(), 0);
}

void TonemapPass::onDebugUi() noexcept {
  i32 op = static_cast<i32>(op_);
  std::array<const char*, 3> labels = {"AgX", "ACES Fitted", "Reinhard-Jodie"};
  if (ImGui::Combo("Operator", &op, labels.data(), labels.size())) {
    op_ = static_cast<Operator>(op);
  }
  f32 exposure = exposure_;
  if (ImGui::SliderFloat("Exposure", &exposure, 0.0f, 8.0f, "%.2f")) {
    exposure_ = exposure;
  }
  ImGui::TextWrapped(
      "Maps HDR scene-referred linear color to sRGB LDR. AgX is the default; "
      "try ACES for a filmic look or Reinhard for a simple neutral response.");
}

}  // namespace uinta
