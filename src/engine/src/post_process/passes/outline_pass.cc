#include "uinta/post_process/passes/outline_pass.h"

#include <imgui.h>

#include <algorithm>
#include <glm/vec2.hpp>

#include "uinta/fbo.h"
#include "uinta/post_process/render_target.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

OutlinePass::OutlinePass(const OpenGLApi* gl) noexcept
    : gl_(gl),
      shader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER, "shader/post_process/outline.fs.glsl"},
          },
          gl),
      uInput_{"uInput", &shader_},
      uDepth_{"uDepth", &shader_},
      uInvProjection_{"uInvProjection", &shader_},
      uTexelSize_{"uTexelSize", &shader_},
      uOutlineColor_{"uOutlineColor", &shader_},
      uThickness_{"uThickness", &shader_},
      uDepthSensitivity_{"uDepthSensitivity", &shader_},
      uNormalSensitivity_{"uNormalSensitivity", &shader_},
      uNear_{"uNear", &shader_},
      uFar_{"uFar", &shader_},
      uFarFadeStart_{"uFarFadeStart", &shader_} {
  ShaderGuard guard(&shader_);
  uInput_ = 0;
  uDepth_ = 1;
}

void OutlinePass::execute(const PassContext& ctx) noexcept {
  if (ctx.input == nullptr || ctx.sceneTarget == nullptr ||
      ctx.sceneTarget->depth() == nullptr) {
    return;
  }

  gl_->bindFramebuffer(GL_FRAMEBUFFER,
                       (ctx.output != nullptr) ? ctx.output->id() : 0);
  gl_->viewport(0, 0, static_cast<GLsizei>(ctx.width),
                static_cast<GLsizei>(ctx.height));

  ShaderGuard shaderGuard(&shader_);
  VaoGuard vaoGuard(ctx.fullscreenVao);

  uInvProjection_ = ctx.invProjection;
  uTexelSize_ =
      glm::vec2(1.0f / static_cast<f32>(std::max<u32>(1, ctx.width)),
                1.0f / static_cast<f32>(std::max<u32>(1, ctx.height)));
  uOutlineColor_ = color_;
  uThickness_ = thickness_;
  uDepthSensitivity_ = depthSensitivity_;
  uNormalSensitivity_ = normalSensitivity_;
  uNear_ = ctx.near;
  uFar_ = ctx.far;
  uFarFadeStart_ = farFadeStart_;

  const Texture* depth = ctx.sceneTarget->depth();
  gl_->activeTexture(GL_TEXTURE1);
  gl_->bindTexture(depth->target(), depth->id());
  gl_->activeTexture(GL_TEXTURE0);
  gl_->bindTexture(ctx.input->target(), ctx.input->id());

  gl_->drawArrays(GL_TRIANGLES, 0, 3);

  gl_->bindTexture(ctx.input->target(), 0);
  gl_->activeTexture(GL_TEXTURE1);
  gl_->bindTexture(depth->target(), 0);
  gl_->activeTexture(GL_TEXTURE0);
}

void OutlinePass::onDebugUi() noexcept {
  glm::vec3 color = {color_.r, color_.g, color_.b};
  if (ImGui::ColorEdit3("Color", &color.r)) {
    color_ = color;
  }
  float t = thickness_;
  if (ImGui::SliderFloat("Thickness", &t, 0.0f, 4.0f, "%.2f")) {
    thickness_ = t;
  }
  float ds = depthSensitivity_;
  if (ImGui::SliderFloat("Depth Sensitivity", &ds, 0.0f, 10.0f, "%.2f")) {
    depthSensitivity_ = ds;
  }
  float ns = normalSensitivity_;
  if (ImGui::SliderFloat("Normal Sensitivity", &ns, 0.0f, 10.0f, "%.2f")) {
    normalSensitivity_ = ns;
  }
  float ff = farFadeStart_;
  if (ImGui::SliderFloat("Far Fade Start", &ff, 0.0f, 1.0f, "%.2f")) {
    farFadeStart_ = ff;
  }
  ImGui::TextWrapped(
      "Cel/toon outline from depth and reconstructed normals. Increase depth "
      "sensitivity for stronger silhouettes; increase normal sensitivity for "
      "inner creases. 'Far Fade Start' is the fraction of the far plane at "
      "which outlines begin fading to zero (1.0 disables the fade).");
}

}  // namespace uinta
