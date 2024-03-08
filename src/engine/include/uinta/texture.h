#ifndef SRC_ENGINE_INCLUDE_UINTA_TEXTURE_H_
#define SRC_ENGINE_INCLUDE_UINTA_TEXTURE_H_

#include <string>

#include "uinta/file.h"
#include "uinta/gl.h"
#include "uinta/lib/stb_image_api.h"
#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

class Texture {
  friend class TextureGuard;

 public:
  Texture(GLenum target = GL_TEXTURE_2D, i32 width = 0, i32 height = 0,
          i32 channels = 0, i32 filter = 0, i32 wrap = 0,
          const OpenGLApi* = OpenGLApiImpl::Instance()) noexcept;

  ~Texture() noexcept {
    gl_->deleteTextures(1, &id_);
    id_ = 0;
    width_ = 0;
    height_ = 0;
  }

  Texture(const Texture&) noexcept = delete;
  Texture& operator=(const Texture&) noexcept = delete;
  Texture(Texture&&) noexcept = delete;
  Texture& operator=(Texture&&) noexcept = delete;

  Status fromFile(
      std::string path, FileSystem* = FileSystemImpl::Instance(),
      const StbImageApi* imageApi = StbImageApiImpl::Instance()) noexcept;

  void filtering(GLint min, GLint mag) noexcept {
    filterMin_ = min;
    filterMag_ = mag;
    gl_->texParameteri(target_, GL_TEXTURE_MIN_FILTER, min);
    gl_->texParameteri(target_, GL_TEXTURE_MAG_FILTER, mag);
  }

  void wrapping(GLint wrap) noexcept {
    wrap_ = wrap;
    gl_->texParameteri(target_, GL_TEXTURE_WRAP_S, wrap_);
    gl_->texParameteri(target_, GL_TEXTURE_WRAP_T, wrap_);
    if (target_ == GL_TEXTURE_3D)
      gl_->texParameteri(target_, GL_TEXTURE_WRAP_R, wrap_);
  }

  GLuint id() const noexcept { return id_; }

  GLenum target() const noexcept { return target_; }

  i32 width() const noexcept { return width_; }

  i32 height() const noexcept { return height_; }

  i32 channels() const noexcept { return channels_; }

  i32 filterMag() const noexcept { return filterMag_; }

  i32 filterMin() const noexcept { return filterMin_; }

  i32 wrap() const noexcept { return wrap_; }

 private:
  GLuint id_ = 0;
  GLenum target_;
  i32 width_ = 0;
  i32 height_ = 0;
  i32 channels_ = 0;
  i32 filterMag_ = 0;
  i32 filterMin_ = 0;
  i32 wrap_ = 0;
  const OpenGLApi* gl_;

  void setStorage(i32 width, i32 height) noexcept {
    width_ = width;
    height_ = height;
    gl_->texImage2D(target_, 0, GL_RGB, width_, height_, 0, GL_RGB,
                    GL_UNSIGNED_BYTE, nullptr);
  }
};

class ActiveTextureGuard {
 public:
  explicit ActiveTextureGuard(
      GLenum texture = GL_TEXTURE0,
      const OpenGLApi* gl = OpenGLApiImpl::Instance()) noexcept
      : gl_(gl) {
    gl_->getIntegerv(GL_ACTIVE_TEXTURE, &prevTextureUnit_);
    gl_->activeTexture(texture);
  }

  ~ActiveTextureGuard() noexcept { gl_->activeTexture(prevTextureUnit_); }

  ActiveTextureGuard(const ActiveTextureGuard&) noexcept = delete;
  ActiveTextureGuard& operator=(const ActiveTextureGuard&) noexcept = delete;
  ActiveTextureGuard(ActiveTextureGuard&&) noexcept = delete;
  ActiveTextureGuard& operator=(ActiveTextureGuard&&) noexcept = delete;

 private:
  GLint prevTextureUnit_;
  const OpenGLApi* gl_;
};

class TextureGuard {
 public:
  explicit TextureGuard(const Texture* texture) noexcept : texture_(texture) {
    texture_->gl_->bindTexture(texture->target_, texture->id());
  }

  ~TextureGuard() noexcept { texture_->gl_->bindTexture(texture_->target_, 0); }

  TextureGuard(const TextureGuard&) noexcept = delete;
  TextureGuard& operator=(const TextureGuard&) noexcept = delete;
  TextureGuard(TextureGuard&&) noexcept = delete;
  TextureGuard& operator=(TextureGuard&&) noexcept = delete;

 private:
  const Texture* texture_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_TEXTURE_H_
