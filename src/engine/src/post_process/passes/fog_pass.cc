#include "uinta/post_process/passes/fog_pass.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "uinta/fbo.h"
#include "uinta/post_process/render_target.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

FogPass::FogPass(const OpenGLApi* gl) noexcept
    : gl_(gl),
      shader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER, "shader/post_process/fog.fs.glsl"},
          },
          gl),
      uInput_{"uInput", &shader_},
      uDepth_{"uDepth", &shader_},
      uInvViewProjection_{"uInvViewProjection", &shader_},
      uCameraPos_{"uCameraPos", &shader_},
      uFogColor_{"uFogColor", &shader_},
      uDensity_{"uDensity", &shader_},
      uNear_{"uNear", &shader_},
      uFar_{"uFar", &shader_},
      uFarFadeStart_{"uFarFadeStart", &shader_} {
  ShaderGuard guard(&shader_);
  uInput_ = 0;
  uDepth_ = 1;
}

void FogPass::execute(const PassContext& ctx) noexcept {
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

  uInvViewProjection_ = ctx.invViewProjection;
  // Extract camera position from inverse view matrix: the inverse view's
  // fourth column is the world-space camera position.
  const glm::vec3 cameraPos = glm::vec3(ctx.invView[3]);
  uCameraPos_ = cameraPos;
  uFogColor_ = color_;
  uDensity_ = density_;
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

void FogPass::onDebugUi() noexcept {
  glm::vec3 color = {color_.r, color_.g, color_.b};
  if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) {
    color_ = color;
  }
  float d = density_;
  if (ImGui::SliderFloat("Density", &d, 0.0f, 0.2f, "%.4f",
                         ImGuiSliderFlags_Logarithmic)) {
    density_ = d;
  }
  float ff = farFadeStart_;
  if (ImGui::SliderFloat("Far Fade Start", &ff, 0.0f, 1.0f, "%.2f")) {
    farFadeStart_ = ff;
  }
  ImGui::TextWrapped(
      "Depth-based exponential fog. Set density to a small value (try 0.01). "
      "'Far Fade Start' is the fraction of the far plane at which fog "
      "smoothly saturates to its color (1.0 disables the fade).");
}

}  // namespace uinta
