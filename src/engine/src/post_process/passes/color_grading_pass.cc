#include "uinta/post_process/passes/color_grading_pass.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>
#include <imgui.h>

#include "uinta/fbo.h"
#include "uinta/vao.h"

namespace uinta {

ColorGradingPass::ColorGradingPass(const OpenGLApi* gl) noexcept
    : gl_(gl),
      shader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER, "shader/post_process/color_grading.fs.glsl"},
          },
          gl),
      uInput_{"uInput", &shader_},
      uLut_{"uLut", &shader_},
      uLutSize_{"uLutSize", &shader_},
      uIntensity_{"uIntensity", &shader_} {
  ShaderGuard guard(&shader_);
  uInput_ = 0;
  uLut_ = 1;
  uLutSize_ = 0.0f;
  uIntensity_ = intensity_;
  enabled(false);
}

Status ColorGradingPass::setLut(const std::string& resourcePath) noexcept {
  // Allocate a fresh texture and load the PNG. fromFile currently uses
  // GL_RGB storage which is fine for LUTs (alpha unused). Filtering set to
  // LINEAR so adjacent cells interpolate correctly; wrap CLAMP_TO_EDGE.
  auto tex = std::make_unique<Texture>(GL_TEXTURE_2D, 0, 0, 0, GL_LINEAR,
                                       GL_CLAMP_TO_EDGE, gl_);
  if (auto status = tex->fromFile(resourcePath); !status.ok()) {
    LOG(ERROR) << "ColorGradingPass: failed to load LUT '" << resourcePath
               << "': " << status;
    return status;
  }
  // After fromFile, width/height/channels are populated.
  const i32 w = tex->width();
  const i32 h = tex->height();
  if (h <= 0 || w != h * h) {
    return InvalidArgumentError(absl::StrFormat(
        "LUT '%s' has invalid dimensions %dx%d; expected (N*N) x N "
        "(i.e. width == height * height).",
        resourcePath, w, h));
  }
  lut_ = std::move(tex);
  lutSize_ = static_cast<u32>(h);
  lutPath_ = resourcePath;
  LOG(INFO) << "ColorGradingPass: loaded LUT '" << resourcePath << "' (size "
            << lutSize_ << ")";
  return OkStatus();
}

void ColorGradingPass::clearLut() noexcept {
  lut_.reset();
  lutSize_ = 0;
  lutPath_.clear();
}

void ColorGradingPass::execute(const PassContext& ctx) noexcept {
  if (ctx.input == nullptr) {
    return;
  }

  gl_->bindFramebuffer(GL_FRAMEBUFFER,
                       (ctx.output != nullptr) ? ctx.output->id() : 0);
  gl_->viewport(0, 0, static_cast<GLsizei>(ctx.width),
                static_cast<GLsizei>(ctx.height));

  ShaderGuard shaderGuard(&shader_);
  VaoGuard vaoGuard(ctx.fullscreenVao);

  uLutSize_ = static_cast<f32>(lutSize_);
  uIntensity_ = intensity_;

  // Unit 0 = scene input, unit 1 = LUT (bound only if available).
  if (lut_ != nullptr) {
    gl_->activeTexture(GL_TEXTURE1);
    gl_->bindTexture(lut_->target(), lut_->id());
  }
  gl_->activeTexture(GL_TEXTURE0);
  gl_->bindTexture(ctx.input->target(), ctx.input->id());

  gl_->drawArrays(GL_TRIANGLES, 0, 3);

  gl_->bindTexture(ctx.input->target(), 0);
  if (lut_ != nullptr) {
    gl_->activeTexture(GL_TEXTURE1);
    gl_->bindTexture(lut_->target(), 0);
    gl_->activeTexture(GL_TEXTURE0);
  }
}

void ColorGradingPass::onDebugUi() noexcept {
  if (lut_ == nullptr) {
    ImGui::TextWrapped(
        "No LUT loaded. Call setLut(\"texture/lut/name.png\") to enable "
        "grading. The PNG must be a (N*N) x N strip (e.g. 256 x 16).");
  } else {
    ImGui::Text("LUT: %s (size %u)", lutPath_.c_str(), lutSize_);
  }
  float intensity = intensity_;
  if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f, "%.2f")) {
    intensity_ = intensity;
  }
}

}  // namespace uinta
