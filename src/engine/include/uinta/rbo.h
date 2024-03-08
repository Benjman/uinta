#ifndef SRC_ENGINE_INCLUDE_UINTA_RBO_H_
#define SRC_ENGINE_INCLUDE_UINTA_RBO_H_

#include "uinta/gl.h"
#include "uinta/types.h"

namespace uinta {

class Rbo {
  friend class RboGuard;

 public:
  Rbo(i32 width, i32 height,
      const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  ~Rbo() noexcept;

  Rbo(const Rbo&) noexcept = delete;
  Rbo& operator=(const Rbo&) noexcept = delete;

  Rbo(Rbo&&) noexcept;
  Rbo& operator=(Rbo&&) noexcept;

  GLuint id() const noexcept { return id_; }

  GLenum target() const noexcept { return target_; }

  GLenum internalFormat() const noexcept { return internalFormat_; }

  i32 width() const noexcept { return width_; }

  i32 height() const noexcept { return height_; }

 private:
  GLuint id_;
  GLenum target_;
  GLenum internalFormat_;
  i32 width_;
  i32 height_;
  const OpenGLApi* gl_;
};

class RboGuard {
 public:
  explicit RboGuard(const Rbo* rbo) noexcept
      : target_(rbo->target()), gl_(rbo->gl_) {
    gl_->bindFramebuffer(target_, rbo->id());
  }

  ~RboGuard() noexcept { gl_->bindFramebuffer(target_, 0); }

  RboGuard(const RboGuard&) noexcept = delete;
  RboGuard& operator=(const RboGuard&) noexcept = delete;
  RboGuard(RboGuard&&) noexcept = delete;
  RboGuard& operator=(RboGuard&&) noexcept = delete;

 private:
  const GLenum target_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_RBO_H_
