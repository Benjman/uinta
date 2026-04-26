#include "uinta/post_process/passes/depth_visualizer_pass.h"

#include <imgui.h>

#include "uinta/fbo.h"
#include "uinta/post_process/render_target.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

DepthVisualizerPass::DepthVisualizerPass(const OpenGLApi* gl) noexcept
    : gl_(gl),
      shader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER,
               "shader/post_process/depth_visualizer.fs.glsl"},
          },
          gl),
      uDepth_{"uDepth", &shader_},
      uNear_{"uNear", &shader_},
      uFar_{"uFar", &shader_} {
  ShaderGuard guard(&shader_);
  uDepth_ = 0;
  uNear_ = 0.1f;
  uFar_ = 100.0f;
  enabled(false);
}

void DepthVisualizerPass::execute(const PassContext& ctx) noexcept {
  if (ctx.sceneTarget == nullptr || ctx.sceneTarget->depth() == nullptr) {
    return;
  }

  gl_->bindFramebuffer(GL_FRAMEBUFFER,
                       (ctx.output != nullptr) ? ctx.output->id() : 0);
  gl_->viewport(0, 0, static_cast<GLsizei>(ctx.width),
                static_cast<GLsizei>(ctx.height));

  ShaderGuard shaderGuard(&shader_);
  VaoGuard vaoGuard(ctx.fullscreenVao);

  uNear_ = ctx.near;
  uFar_ = ctx.far;
  lastNear_ = ctx.near;
  lastFar_ = ctx.far;

  ActiveTextureGuard activeGuard(GL_TEXTURE0, gl_);
  const Texture* depth = ctx.sceneTarget->depth();
  gl_->bindTexture(depth->target(), depth->id());

  gl_->drawArrays(GL_TRIANGLES, 0, 3);

  gl_->bindTexture(depth->target(), 0);
}

void DepthVisualizerPass::onDebugUi() noexcept {
  ImGui::Text("Linearized depth (near=%.2f far=%.1f)", lastNear_, lastFar_);
  ImGui::TextWrapped(
      "Grayscale visualization of the simulation depth buffer. White = far.");
}

}  // namespace uinta
