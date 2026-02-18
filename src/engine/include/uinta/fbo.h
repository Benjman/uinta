#ifndef SRC_ENGINE_INCLUDE_UINTA_FBO_H_
#define SRC_ENGINE_INCLUDE_UINTA_FBO_H_

#include <vector>

#include "uinta/flags.h"
#include "uinta/gl.h"
#include "uinta/texture.h"

namespace uinta {

class Fbo {
  friend class FboGuard;

 public:
  struct Flags final {
    using value_type = u8;

    static constexpr value_type DepthAttachmentMask = 1 << 0;
    static constexpr value_type DepthStencilAttachmentMask = 1 << 1;

    static constexpr value_type DefaultFlags = 0;

    FlagsOperations(DefaultFlags);

    bool hasDepthAttachment() const noexcept {
      return flags_ & DepthAttachmentMask;
    }
    void hasDepthAttachment(bool v) noexcept {
      flags_ &= ~DepthAttachmentMask;
      if (v) flags_ |= DepthAttachmentMask;
    }

    bool hasDepthStencilAttachment() const noexcept {
      return flags_ & DepthStencilAttachmentMask;
    }
    void hasDepthStencilAttachment(bool v) noexcept {
      flags_ &= ~DepthStencilAttachmentMask;
      if (v) flags_ |= DepthStencilAttachmentMask;
    }

   private:
    value_type flags_;
  };

  const Flags& flags() const noexcept { return flags_; }
  Flags& flags() noexcept { return flags_; }

  explicit Fbo(const OpenGLApi* gl = OpenGLApiImpl::Instance()) noexcept
      : gl_(gl) {
    gl_->genFramebuffers(1, &id_);
  }

  ~Fbo() noexcept {
    gl_->deleteFramebuffers(1, &id_);
    gl_ = nullptr;
    id_ = 0;
  }

  Fbo(const Fbo&) noexcept = delete;
  Fbo& operator=(const Fbo&) noexcept = delete;
  Fbo(Fbo&&) noexcept = delete;
  Fbo& operator=(Fbo&&) noexcept = delete;

  // Attach a texture to a color attachment point
  void attachColorTexture(const Texture* texture, GLuint colorAttachment = 0,
                          GLint level = 0) noexcept {
    gl_->framebufferTexture2D(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0 + colorAttachment,
                              texture->target(), texture->id(), level);
    if (colorAttachment >= colorAttachments_.size()) {
      colorAttachments_.resize(colorAttachment + 1);
    }
    colorAttachments_[colorAttachment] = GL_COLOR_ATTACHMENT0 + colorAttachment;
  }

  // Attach a texture to the depth attachment point
  void attachDepthTexture(const Texture* texture, GLint level = 0) noexcept {
    gl_->framebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              texture->target(), texture->id(), level);
    flags_.hasDepthAttachment(true);
  }

  // Attach a texture to the depth-stencil attachment point
  void attachDepthStencilTexture(const Texture* texture,
                                 GLint level = 0) noexcept {
    gl_->framebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              texture->target(), texture->id(), level);
    flags_.hasDepthStencilAttachment(true);
  }

  // Attach a renderbuffer to a color attachment point
  void attachColorRenderbuffer(GLuint renderbuffer,
                               GLuint colorAttachment = 0) noexcept {
    gl_->framebufferRenderbuffer(GL_FRAMEBUFFER,
                                 GL_COLOR_ATTACHMENT0 + colorAttachment,
                                 GL_RENDERBUFFER, renderbuffer);
    if (colorAttachment >= colorAttachments_.size()) {
      colorAttachments_.resize(colorAttachment + 1);
    }
    colorAttachments_[colorAttachment] = GL_COLOR_ATTACHMENT0 + colorAttachment;
  }

  // Attach a renderbuffer to the depth attachment point
  void attachDepthRenderbuffer(GLuint renderbuffer) noexcept {
    gl_->framebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, renderbuffer);
    flags_.hasDepthAttachment(true);
  }

  // Attach a renderbuffer to the depth-stencil attachment point
  void attachDepthStencilRenderbuffer(GLuint renderbuffer) noexcept {
    gl_->framebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                 GL_RENDERBUFFER, renderbuffer);
    flags_.hasDepthStencilAttachment(true);
  }

  // Set which color attachments to draw to
  void setDrawBuffers() const noexcept {
    if (!colorAttachments_.empty()) {
      gl_->drawBuffers(static_cast<GLsizei>(colorAttachments_.size()),
                       colorAttachments_.data());
    }
  }

  // Check if the framebuffer is complete
  GLenum checkStatus() const noexcept {
    return gl_->checkFramebufferStatus(GL_FRAMEBUFFER);
  }

  // Check if the framebuffer is complete and return success status
  bool isComplete() const noexcept {
    return checkStatus() == GL_FRAMEBUFFER_COMPLETE;
  }

  GLuint id() const noexcept { return id_; }

 private:
  GLuint id_ = 0;
  std::vector<GLenum> colorAttachments_;
  const OpenGLApi* gl_;
  Flags flags_;
};

class FboGuard {
 public:
  explicit FboGuard(const Fbo* fbo, bool isActive = true) noexcept
      : id_(fbo->id_), gl_(fbo->gl_) {
    if (isActive) activate();
  }

  ~FboGuard() noexcept {
    if (isActive_) gl_->bindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  FboGuard(const FboGuard&) noexcept = delete;
  FboGuard& operator=(const FboGuard&) noexcept = delete;
  FboGuard(FboGuard&&) noexcept = delete;
  FboGuard& operator=(FboGuard&&) noexcept = delete;

 private:
  const GLuint id_;
  bool isActive_ = false;
  const OpenGLApi* gl_;

  void activate() noexcept {
    gl_->bindFramebuffer(GL_FRAMEBUFFER, id_);
    isActive_ = true;
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_FBO_H_
