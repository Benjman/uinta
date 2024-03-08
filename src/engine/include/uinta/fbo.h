#ifndef SRC_ENGINE_INCLUDE_UINTA_FBO_H_
#define SRC_ENGINE_INCLUDE_UINTA_FBO_H_

#include "uinta/gl.h"
#include "uinta/rbo.h"
#include "uinta/texture.h"

namespace uinta {

class Fbo {
  friend class FboGuard;

 public:
  Fbo(i32 width = 0, i32 height = 0, GLenum target = GL_FRAMEBUFFER,
      const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  ~Fbo() noexcept;

  Fbo(const Fbo&) noexcept = delete;
  Fbo& operator=(const Fbo&) noexcept = delete;

  Fbo(Fbo&&) noexcept;
  Fbo& operator=(Fbo&&) noexcept;

  const Texture& texture() const noexcept { return texture_; }

  const Rbo& rbo() const noexcept { return rbo_; }

  GLuint id() const noexcept { return id_; }

  GLenum target() const noexcept { return target_; }

  i32 width() const noexcept { return width_; }

  i32 height() const noexcept { return height_; }

 private:
  Texture texture_;
  Rbo rbo_;
  GLuint id_;
  GLenum target_;
  i32 width_;
  i32 height_;
  const OpenGLApi* gl_;
};

class FboGuard {
 public:
  explicit FboGuard(const Fbo* fbo) noexcept
      : target_(fbo->target()), gl_(fbo->gl_) {
    gl_->bindFramebuffer(target_, fbo->id());
  }

  ~FboGuard() noexcept { gl_->bindFramebuffer(target_, 0); }

  FboGuard(const FboGuard&) noexcept = delete;
  FboGuard& operator=(const FboGuard&) noexcept = delete;
  FboGuard(FboGuard&&) noexcept = delete;
  FboGuard& operator=(FboGuard&&) noexcept = delete;

 private:
  const GLenum target_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_FBO_H_
