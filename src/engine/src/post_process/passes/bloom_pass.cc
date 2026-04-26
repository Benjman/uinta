#include "uinta/post_process/passes/bloom_pass.h"

#include <imgui.h>

#include <algorithm>

#include "uinta/fbo.h"
#include "uinta/post_process/blit_helper.h"
#include "uinta/post_process/render_target.h"
#include "uinta/post_process/resource_pool.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

BloomPass::BloomPass(const OpenGLApi* gl) noexcept
    : gl_(gl),
      downsampleShader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER,
               "shader/post_process/bloom_downsample.fs.glsl"},
          },
          gl),
      dsInput_{"uInput", &downsampleShader_},
      dsTexelSize_{"uTexelSize", &downsampleShader_},
      dsIsFirst_{"uIsFirst", &downsampleShader_},
      upsampleShader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER,
               "shader/post_process/bloom_upsample.fs.glsl"},
          },
          gl),
      usInput_{"uInput", &upsampleShader_},
      usFilterRadius_{"uFilterRadius", &upsampleShader_},
      compositeShader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER,
               "shader/post_process/bloom_composite.fs.glsl"},
          },
          gl),
      compInput_{"uInput", &compositeShader_},
      compBloom_{"uBloom", &compositeShader_},
      compIntensity_{"uIntensity", &compositeShader_},
      compThreshold_{"uThreshold", &compositeShader_} {
  {
    ShaderGuard g(&downsampleShader_);
    dsInput_ = 0;
  }
  {
    ShaderGuard g(&upsampleShader_);
    usInput_ = 0;
  }
  {
    ShaderGuard g(&compositeShader_);
    compInput_ = 0;
    compBloom_ = 1;
  }
  enabled(false);
}

void BloomPass::execute(const PassContext& ctx) noexcept {
  if (ctx.input == nullptr || ctx.pool == nullptr ||
      ctx.fullscreenVao == nullptr) {
    return;
  }
  if (intensity_ <= 0.0f) {
    // Bypass: just copy input to output via the shared blit.
    if (ctx.blit != nullptr) {
      ctx.blit->draw(ctx.input, ctx.output, ctx.width, ctx.height);
    }
    return;
  }

  // Acquire a mip-chain auxiliary target.
  ResourceDesc desc;
  desc.width = ctx.width;
  desc.height = ctx.height;
  desc.color = RenderTargetFormats::Hdr();
  desc.color.levels = std::clamp<u32>(mipCount_, 2, 8);
  desc.debugName = "bloom::mipchain";
  RenderTarget* mipTarget = ctx.pool->acquire(desc);
  if (mipTarget == nullptr || mipTarget->fbo() == nullptr) {
    if (ctx.blit != nullptr) {
      ctx.blit->draw(ctx.input, ctx.output, ctx.width, ctx.height);
    }
    return;
  }

  const u32 levels = mipTarget->mipCount();

  // --- Downsample chain -------------------------------------------------
  {
    ShaderGuard shaderGuard(&downsampleShader_);
    VaoGuard vaoGuard(ctx.fullscreenVao);
    ActiveTextureGuard activeGuard(GL_TEXTURE0, gl_);

    for (u32 mip = 0; mip < levels; ++mip) {
      const u32 srcW = (mip == 0) ? ctx.width : mipTarget->width(mip - 1);
      const u32 srcH = (mip == 0) ? ctx.height : mipTarget->height(mip - 1);
      const Texture* srcTex = (mip == 0) ? ctx.input : mipTarget->color();

      // For mip 0 we sample the full-res input texture (LOD 0 implicit).
      // For mip N > 0 we sample the mipTarget's color texture and need to
      // select the previous mip. GL requires setting BASE/MAX_LEVEL bounds
      // via texParameteri so texture() reads from the correct LOD.
      gl_->bindTexture(srcTex->target(), srcTex->id());
      if (mip > 0) {
        gl_->texParameteri(srcTex->target(), GL_TEXTURE_BASE_LEVEL,
                           static_cast<GLint>(mip - 1));
        gl_->texParameteri(srcTex->target(), GL_TEXTURE_MAX_LEVEL,
                           static_cast<GLint>(mip - 1));
      }

      const Fbo* dst = mipTarget->fbo(mip);
      gl_->bindFramebuffer(GL_FRAMEBUFFER, (dst != nullptr) ? dst->id() : 0);
      gl_->viewport(0, 0, static_cast<GLsizei>(mipTarget->width(mip)),
                    static_cast<GLsizei>(mipTarget->height(mip)));

      dsTexelSize_ = glm::vec2(1.0f / static_cast<f32>(srcW),
                               1.0f / static_cast<f32>(srcH));
      dsIsFirst_ = (mip == 0) ? 1 : 0;

      gl_->drawArrays(GL_TRIANGLES, 0, 3);

      if (mip > 0) {
        // Restore default mip range for subsequent use.
        gl_->texParameteri(srcTex->target(), GL_TEXTURE_BASE_LEVEL, 0);
        gl_->texParameteri(srcTex->target(), GL_TEXTURE_MAX_LEVEL,
                           static_cast<GLint>(levels - 1));
      }
    }
    gl_->bindTexture(GL_TEXTURE_2D, 0);
  }

  // --- Upsample chain (additive) ---------------------------------------
  {
    ShaderGuard shaderGuard(&upsampleShader_);
    VaoGuard vaoGuard(ctx.fullscreenVao);
    ActiveTextureGuard activeGuard(GL_TEXTURE0, gl_);

    // Additive blending: writes += existing contents.
    BlendGuard blendGuard(GL_ONE, GL_ONE, /*isActive=*/true, gl_);

    const Texture* chain = mipTarget->color();
    gl_->bindTexture(chain->target(), chain->id());

    // Upsample from deepest mip to mip 0: for each src mip k, draw into
    // mip k-1 sampling mip k.
    for (u32 mip = levels - 1; mip > 0; --mip) {
      gl_->texParameteri(chain->target(), GL_TEXTURE_BASE_LEVEL,
                         static_cast<GLint>(mip));
      gl_->texParameteri(chain->target(), GL_TEXTURE_MAX_LEVEL,
                         static_cast<GLint>(mip));

      const Fbo* dst = mipTarget->fbo(mip - 1);
      gl_->bindFramebuffer(GL_FRAMEBUFFER, (dst != nullptr) ? dst->id() : 0);
      gl_->viewport(0, 0, static_cast<GLsizei>(mipTarget->width(mip - 1)),
                    static_cast<GLsizei>(mipTarget->height(mip - 1)));

      usFilterRadius_ = filterRadius_;

      gl_->drawArrays(GL_TRIANGLES, 0, 3);
    }

    // Restore default mip range.
    gl_->texParameteri(chain->target(), GL_TEXTURE_BASE_LEVEL, 0);
    gl_->texParameteri(chain->target(), GL_TEXTURE_MAX_LEVEL,
                       static_cast<GLint>(levels - 1));
    gl_->bindTexture(chain->target(), 0);
  }

  // --- Composite into output ------------------------------------------
  {
    ShaderGuard shaderGuard(&compositeShader_);
    VaoGuard vaoGuard(ctx.fullscreenVao);

    gl_->bindFramebuffer(GL_FRAMEBUFFER,
                         (ctx.output != nullptr) ? ctx.output->id() : 0);
    gl_->viewport(0, 0, static_cast<GLsizei>(ctx.width),
                  static_cast<GLsizei>(ctx.height));

    compIntensity_ = intensity_;
    compThreshold_ = threshold_;

    const Texture* bloomTex = mipTarget->color();

    gl_->activeTexture(GL_TEXTURE1);
    gl_->bindTexture(bloomTex->target(), bloomTex->id());
    gl_->activeTexture(GL_TEXTURE0);
    gl_->bindTexture(ctx.input->target(), ctx.input->id());

    gl_->drawArrays(GL_TRIANGLES, 0, 3);

    gl_->bindTexture(ctx.input->target(), 0);
    gl_->activeTexture(GL_TEXTURE1);
    gl_->bindTexture(bloomTex->target(), 0);
    gl_->activeTexture(GL_TEXTURE0);
  }

  ctx.pool->release(mipTarget);
}

void BloomPass::onDebugUi() noexcept {
  f32 intensity = intensity_;
  if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 2.0f, "%.2f")) {
    intensity_ = intensity;
  }
  f32 threshold = threshold_;
  if (ImGui::SliderFloat("Threshold", &threshold, 0.0f, 4.0f, "%.2f")) {
    threshold_ = threshold;
  }
  f32 radius = filterRadius_;
  if (ImGui::SliderFloat("Filter Radius", &radius, 0.001f, 0.02f, "%.4f")) {
    filterRadius_ = radius;
  }
  i32 mips = static_cast<i32>(mipCount_);
  if (ImGui::SliderInt("Mip Count", &mips, 2, 8)) {
    mipCount_ = static_cast<u32>(mips);
  }
  ImGui::TextWrapped(
      "Dual-filter bloom (Jimenez 2014). Runs a %u-level downsample/upsample "
      "chain and composites onto the scene additively. Set intensity to 0 to "
      "bypass.",
      mipCount_);
}

}  // namespace uinta
