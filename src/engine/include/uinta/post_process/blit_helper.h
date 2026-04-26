#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_BLIT_HELPER_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_BLIT_HELPER_H_

#include "uinta/gl.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/types.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"

namespace uinta {

class Fbo;

/*! Shared helper that draws a source texture into a destination FBO using a
 *  fullscreen oversized-triangle pass. Used both as the fallback pass when no
 *  post-process effects are enabled and as a reusable blit primitive inside
 *  pass implementations.
 */
class BlitHelper {
 public:
  explicit BlitHelper(const OpenGLApi* gl) noexcept;

  BlitHelper(const BlitHelper&) = delete;
  BlitHelper& operator=(const BlitHelper&) = delete;
  BlitHelper(BlitHelper&&) = delete;
  BlitHelper& operator=(BlitHelper&&) = delete;

  /*! Draw `source` into `destFbo` (nullptr = default framebuffer) at the given
   *  viewport dimensions. The caller is responsible for depth-state
   *  management; the blit itself writes at z=0 with no depth interaction. */
  void draw(const Texture* source, const Fbo* destFbo, u32 width,
            u32 height) const noexcept;

  [[nodiscard]] const Vao* vao() const noexcept { return &vao_; }
  [[nodiscard]] const Shader* shader() const noexcept { return &shader_; }

 private:
  const OpenGLApi* gl_;
  Vao vao_;
  Shader shader_;
  Uniform1i uInput_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_BLIT_HELPER_H_
