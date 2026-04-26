#include "uinta/post_process/blit_helper.h"

#include "uinta/fbo.h"

namespace uinta {

BlitHelper::BlitHelper(const OpenGLApi* gl) noexcept
    : gl_(gl),
      vao_(gl),
      shader_(
          {
              {GL_VERTEX_SHADER, "shader/post_process/fullscreen.vs.glsl"},
              {GL_FRAGMENT_SHADER, "shader/post_process/passthrough.fs.glsl"},
          },
          gl),
      uInput_{"uInput", &shader_} {
  ShaderGuard guard(&shader_);
  uInput_ = 0;
}

void BlitHelper::draw(const Texture* source, const Fbo* destFbo, u32 width,
                      u32 height) const noexcept {
  // Bind destination (nullptr = default framebuffer).
  gl_->bindFramebuffer(GL_FRAMEBUFFER,
                       (destFbo != nullptr) ? destFbo->id() : 0);
  gl_->viewport(0, 0, static_cast<GLsizei>(width),
                static_cast<GLsizei>(height));

  ShaderGuard shaderGuard(&shader_);
  VaoGuard vaoGuard(&vao_);

  ActiveTextureGuard activeGuard(GL_TEXTURE0, gl_);
  if (source != nullptr) {
    gl_->bindTexture(source->target(), source->id());
  }

  gl_->drawArrays(GL_TRIANGLES, 0, 3);

  if (source != nullptr) {
    gl_->bindTexture(source->target(), 0);
  }
}

}  // namespace uinta
