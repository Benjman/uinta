#include "uinta/rbo.h"

#include "uinta/gl.h"

namespace uinta {

Rbo::Rbo(i32 width, i32 height, const OpenGLApi* gl) noexcept
    : target_(GL_RENDERBUFFER),
      internalFormat_(GL_DEPTH24_STENCIL8),
      width_(width),
      height_(height),
      gl_(gl) {
  gl_->genRenderbuffers(1, &id_);
  RboGuard rboGuard(this);
  gl_->renderbufferStorage(target_, internalFormat_, width_, height_);
}

Rbo::~Rbo() noexcept { gl_->deleteRenderbuffers(1, &id_); }

Rbo::Rbo(Rbo&& other) noexcept
    : id_(other.id_),
      target_(other.target_),
      internalFormat_(other.internalFormat_),
      width_(other.width_),
      height_(other.height_),
      gl_(other.gl_) {
  other.id_ = 0;
  other.target_ = GL_NONE;
  other.internalFormat_ = GL_NONE;
  other.width_ = 0;
  other.height_ = 0;
  other.gl_ = nullptr;
}

Rbo& Rbo::operator=(Rbo&& other) noexcept {
  if (this != &other) {
    gl_->deleteFramebuffers(1, &id_);
    id_ = other.id_;
    target_ = other.target_;
    internalFormat_ = other.internalFormat_;
    width_ = other.width_;
    height_ = other.height_;
    gl_ = other.gl_;

    other.id_ = 0;
    other.target_ = GL_NONE;
    other.internalFormat_ = GL_NONE;
    other.width_ = 0;
    other.height_ = 0;
    other.gl_ = nullptr;
  }
  return *this;
}

}  // namespace uinta
