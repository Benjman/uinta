#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_RENDER_TARGET_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_RENDER_TARGET_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "uinta/fbo.h"
#include "uinta/gl.h"
#include "uinta/texture.h"
#include "uinta/types.h"

namespace uinta {

/*! Standard color formats for post-process render targets. */
namespace RenderTargetFormats {

inline constexpr TextureFormat Ldr() noexcept {
  TextureFormat f;
  f.internalFormat = GL_RGBA8;
  f.pixelFormat = GL_RGBA;
  f.pixelType = GL_UNSIGNED_BYTE;
  f.levels = 1;
  f.immutable = true;
  return f;
}

inline constexpr TextureFormat Hdr() noexcept {
  TextureFormat f;
  f.internalFormat = GL_RGBA16F;
  f.pixelFormat = GL_RGBA;
  f.pixelType = GL_HALF_FLOAT;
  f.levels = 1;
  f.immutable = true;
  return f;
}

inline constexpr TextureFormat Depth24() noexcept {
  TextureFormat f;
  f.internalFormat = GL_DEPTH_COMPONENT24;
  f.pixelFormat = GL_DEPTH_COMPONENT;
  f.pixelType = GL_FLOAT;
  f.levels = 1;
  f.immutable = true;
  return f;
}

}  // namespace RenderTargetFormats

/*! A single-attachment or color+depth render target.
 *
 *  Owns a color texture, optional depth texture, and one or more `Fbo`s (one
 *  per mip level of the color texture). When `colorMips > 1`, separate FBOs
 *  are allocated that attach the color texture at each mip level; this is the
 *  pattern used by bloom's downsample/upsample chain.
 */
class RenderTarget {
 public:
  RenderTarget(const OpenGLApi* gl, u32 width, u32 height,
               TextureFormat colorFormat,
               std::optional<TextureFormat> depthFormat = std::nullopt,
               std::string debugName = {}) noexcept;

  ~RenderTarget() noexcept = default;

  RenderTarget(const RenderTarget&) = delete;
  RenderTarget& operator=(const RenderTarget&) = delete;
  RenderTarget(RenderTarget&&) = delete;
  RenderTarget& operator=(RenderTarget&&) = delete;

  /*! Reallocate underlying color/depth textures at new dimensions. */
  void resize(u32 width, u32 height) noexcept;

  [[nodiscard]] const Texture* color() const noexcept { return color_.get(); }
  [[nodiscard]] Texture* color() noexcept { return color_.get(); }

  [[nodiscard]] const Texture* depth() const noexcept { return depth_.get(); }
  [[nodiscard]] Texture* depth() noexcept { return depth_.get(); }

  /*! FBO for mip level 0 (the base). */
  [[nodiscard]] Fbo* fbo() noexcept { return fbos_.empty() ? nullptr : fbos_[0].get(); }
  [[nodiscard]] const Fbo* fbo() const noexcept { return fbos_.empty() ? nullptr : fbos_[0].get(); }

  /*! FBO for a specific mip level. `mip == 0` is the base. */
  [[nodiscard]] Fbo* fbo(u32 mip) noexcept;
  [[nodiscard]] const Fbo* fbo(u32 mip) const noexcept;

  [[nodiscard]] u32 width() const noexcept { return width_; }
  [[nodiscard]] u32 height() const noexcept { return height_; }
  [[nodiscard]] u32 width(u32 mip) const noexcept;
  [[nodiscard]] u32 height(u32 mip) const noexcept;
  [[nodiscard]] u32 mipCount() const noexcept { return colorFormat_.levels; }

  [[nodiscard]] const TextureFormat& colorFormat() const noexcept {
    return colorFormat_;
  }

  [[nodiscard]] const std::optional<TextureFormat>& depthFormat() const noexcept {
    return depthFormat_;
  }

  [[nodiscard]] const std::string& debugName() const noexcept {
    return debugName_;
  }

 private:
  const OpenGLApi* gl_;
  u32 width_ = 0;
  u32 height_ = 0;
  TextureFormat colorFormat_;
  std::optional<TextureFormat> depthFormat_;
  std::string debugName_;

  std::unique_ptr<Texture> color_;
  std::unique_ptr<Texture> depth_;
  std::vector<std::unique_ptr<Fbo>> fbos_;  // one per mip level

  void allocate() noexcept;
  void release() noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_RENDER_TARGET_H_
