#include "uinta/fbo.h"

#include "absl/log/log.h"
#include "uinta/gl.h"
#include "uinta/texture.h"

namespace uinta {

Fbo::Fbo(i32 width, i32 height, GLenum target, const OpenGLApi* gl) noexcept
    : texture_(GL_TEXTURE_2D, width, height),
      rbo_(width, height),
      target_(target),
      width_(width),
      height_(height),
      gl_(gl) {
  gl_->genFramebuffers(1, &id_);
  texture_.setStorage(width, height);
  texture_.filtering(GL_LINEAR, GL_LINEAR);

  FboGuard fboGuard(this);
  gl_->framebufferTexture2D(target_, GL_COLOR_ATTACHMENT0, texture_.target(),
                            texture_.id(), 0);
  gl_->framebufferRenderbuffer(target_, GL_DEPTH_STENCIL_ATTACHMENT,
                               rbo_.target(), rbo_.id());

  switch (gl_->checkFramebufferStatus(target_)) {
    case GL_FRAMEBUFFER_COMPLETE:
      // No action on success
      break;
    case GL_FRAMEBUFFER_UNDEFINED:
      LOG(FATAL)
          << "checkFramebufferStatus() returned GL_FRAMEBUFFER_UNDEFINED";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      LOG(FATAL) << "checkFramebufferStatus() returned "
                    "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      LOG(FATAL) << "checkFramebufferStatus() returned "
                    "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      LOG(FATAL) << "checkFramebufferStatus() returned "
                    "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      LOG(FATAL) << "checkFramebufferStatus() returned "
                    "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      LOG(FATAL)
          << "checkFramebufferStatus() returned GL_FRAMEBUFFER_UNSUPPORTED";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      LOG(FATAL) << "checkFramebufferStatus() returned "
                    "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
      LOG(FATAL) << "checkFramebufferStatus() returned "
                    "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
      break;
    default:
      LOG(FATAL) << "checkFramebufferStatus() returned an unknown value: "
                 << gl_->checkFramebufferStatus(target_);
  }
}

Fbo::~Fbo() noexcept { gl_->deleteFramebuffers(1, &id_); }

Fbo::Fbo(Fbo&& other) noexcept
    : texture_(std::move(other.texture_)),
      rbo_(std::move(other.rbo_)),
      id_(other.id_),
      target_(other.target_),
      width_(other.width_),
      height_(other.height_),
      gl_(other.gl_) {
  other.id_ = 0;
  other.target_ = GL_NONE;
  other.width_ = 0;
  other.height_ = 0;
  other.gl_ = nullptr;
}

Fbo& Fbo::operator=(Fbo&& other) noexcept {
  if (this != &other) {
    gl_->deleteFramebuffers(1, &id_);
    texture_ = std::move(other.texture_);
    rbo_ = std::move(other.rbo_);
    id_ = other.id_;
    target_ = other.target_;
    width_ = other.width_;
    height_ = other.height_;
    gl_ = other.gl_;

    other.id_ = 0;
    other.target_ = GL_NONE;
    other.width_ = 0;
    other.height_ = 0;
    other.gl_ = nullptr;
  }
  return *this;
}

}  // namespace uinta
