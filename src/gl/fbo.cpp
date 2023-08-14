#include <uinta/gl/api.h>

#include <uinta/gl/fbo.hpp>
#include <uinta/gl/utils/errors.hpp>

#ifdef UINTA_DEBUG
#include <uinta/logging.hpp>
#endif

namespace uinta {

inline void genColorBufferAttachment(Fbo& fbo);

void initFbo(Fbo& fbo) {
  glGenFramebuffers(1, &fbo.id);
  UINTA_glGetError(glGenFramebuffers);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
  UINTA_glGetError(glBindFramebuffer);
  // color texture
  glGenTextures(1, &fbo.renderTexture);
  UINTA_glGetError(glGenTextures);
  glBindTexture(GL_TEXTURE_2D, fbo.renderTexture);
  UINTA_glGetError(glBindTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo.width, fbo.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  UINTA_glGetError(glTexImage2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  UINTA_glGetError(glTexParameteri);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  UINTA_glGetError(glTexParameteri);
  // depth buffer
  glGenRenderbuffers(1, &fbo.depthBuffer);
  UINTA_glGetError(glGenRenderbuffers);
  glBindRenderbuffer(GL_RENDERBUFFER, fbo.depthBuffer);
  UINTA_glGetError(glBindRenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbo.width, fbo.height);
  UINTA_glGetError(glRenderbufferStorage);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.depthBuffer);
  UINTA_glGetError(glFramebufferRenderbuffer);
  // configure fbo
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo.renderTexture, 0);
  UINTA_glGetError(glFramebufferTexture);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  UINTA_glGetError(glDrawBuffer);
#ifdef UINTA_DEBUG
  auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  UINTA_glGetError(glCheckFramebufferStatus);
  if (status != GL_FRAMEBUFFER_COMPLETE) SPDLOG_ERROR("Incomplete framebuffer!");
#endif
}

void genColorBufferAttachment(Fbo& fbo) {
}

}  // namespace uinta
