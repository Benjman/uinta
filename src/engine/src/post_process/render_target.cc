#include "uinta/post_process/render_target.h"

#include <absl/log/log.h>

#include <algorithm>
#include <utility>

namespace uinta {

RenderTarget::RenderTarget(const OpenGLApi* gl, u32 width, u32 height,
                           TextureFormat colorFormat,
                           std::optional<TextureFormat> depthFormat,
                           std::string debugName) noexcept
    : gl_(gl),
      width_(width),
      height_(height),
      colorFormat_(colorFormat),
      depthFormat_(depthFormat),
      debugName_(std::move(debugName)) {
  if (colorFormat_.levels == 0) {
    colorFormat_.levels = 1;
  }
  if (width_ != 0 && height_ != 0) {
    allocate();
  }
}

void RenderTarget::resize(u32 width, u32 height) noexcept {
  if (width == width_ && height == height_) {
    return;
  }
  release();
  width_ = width;
  height_ = height;
  if (width_ != 0 && height_ != 0) {
    allocate();
  }
}

void RenderTarget::release() noexcept {
  fbos_.clear();
  color_.reset();
  depth_.reset();
}

void RenderTarget::allocate() noexcept {
  // Allocate color texture.
  color_ = std::make_unique<Texture>(
      GL_TEXTURE_2D, static_cast<i32>(width_), static_cast<i32>(height_),
      colorFormat_, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, gl_);

  // Allocate depth texture if requested.
  if (depthFormat_.has_value()) {
    depth_ = std::make_unique<Texture>(
        GL_TEXTURE_2D, static_cast<i32>(width_), static_cast<i32>(height_),
        depthFormat_.value(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, gl_);
  }

  // Allocate FBOs, one per mip level.
  const u32 levels = std::max<u32>(1, colorFormat_.levels);
  fbos_.reserve(levels);
  for (u32 mip = 0; mip < levels; ++mip) {
    auto fbo = std::make_unique<Fbo>(gl_);
    {
      FboGuard guard(fbo.get());
      fbo->attachColorTexture(color_.get(), 0, static_cast<GLint>(mip));
      if (mip == 0 && depth_) {
        fbo->attachDepthTexture(depth_.get());
      }
      fbo->setDrawBuffers();
      if (!fbo->isComplete()) {
        LOG(ERROR) << "RenderTarget '" << debugName_
                   << "' FBO incomplete at mip " << mip << " (status=0x"
                   << std::hex << fbo->checkStatus() << std::dec << ")";
      }
    }
    fbos_.push_back(std::move(fbo));
  }
}

Fbo* RenderTarget::fbo(u32 mip) noexcept {
  if (mip >= fbos_.size()) {
    return nullptr;
  }
  return fbos_[mip].get();
}

const Fbo* RenderTarget::fbo(u32 mip) const noexcept {
  if (mip >= fbos_.size()) {
    return nullptr;
  }
  return fbos_[mip].get();
}

u32 RenderTarget::width(u32 mip) const noexcept {
  return std::max<u32>(1, width_ >> mip);
}

u32 RenderTarget::height(u32 mip) const noexcept {
  return std::max<u32>(1, height_ >> mip);
}

}  // namespace uinta
