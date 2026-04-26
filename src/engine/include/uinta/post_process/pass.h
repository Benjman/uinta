#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASS_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASS_H_

#include <glm/ext/matrix_float4x4.hpp>
#include <string_view>

#include "uinta/gl.h"
#include "uinta/types.h"

namespace uinta {

class BlitHelper;
class Fbo;
class RenderTarget;
class ResourcePool;
class Texture;
class Vao;

/*! Per-frame context handed to each `PostProcessPass::execute`.
 *
 *  Passes consume `input` and render into `output` (or the default framebuffer
 *  when `output == nullptr`). Matrix state is populated from CameraManager /
 *  ViewportManager by `PostProcessor` each frame and is used by passes that
 *  reconstruct world-space positions from scene depth (fog, outline, SSR...).
 */
struct PassContext {
  const OpenGLApi* gl = nullptr;

  // Shared resources (owned by the PostProcessor).
  const Vao* fullscreenVao = nullptr;
  const BlitHelper* blit = nullptr;

  // Viewport dimensions at full resolution.
  u32 width = 0;
  u32 height = 0;

  // Camera clip planes.
  f32 near = 0.1f;
  f32 far = 100.0f;

  // Time in seconds since engine start (wall-clock, for shader animations).
  time_t runtime = 0;

  // The primary simulation target (color + depth). Always available.
  const RenderTarget* sceneTarget = nullptr;

  // The texture this pass should sample as "input". For the first enabled
  // pass, this is `sceneTarget`'s color attachment; for subsequent passes it
  // is the output of the previous enabled pass.
  const Texture* input = nullptr;

  // Destination framebuffer. `nullptr` means the default framebuffer (the
  // terminal pass); UI/Debug layers composite on top of this.
  const Fbo* output = nullptr;

  // Camera matrices, populated by PostProcessor each frame. Required by
  // passes that reconstruct world-space positions from depth (fog, outline,
  // SSR, etc.).
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  glm::mat4 invView = glm::mat4(1.0f);
  glm::mat4 invProjection = glm::mat4(1.0f);
  glm::mat4 invViewProjection = glm::mat4(1.0f);

  // Transient resource pool. Passes that need intermediate mip chains or
  // auxiliary targets should acquire/release handles here rather than owning
  // their own targets.
  ResourcePool* pool = nullptr;
};

/*! Abstract post-process pass. Subclass to implement an effect.
 *
 *  Execution contract:
 *    * `execute()` must leave GL state unchanged aside from the effect itself.
 *    * Depth test is disabled by the PostProcessor before dispatching passes.
 *    * The PostProcessor binds no framebuffer; the pass must bind
 *      `ctx.output` (or FBO 0 when `ctx.output` is null) at the start.
 *    * The shared fullscreen VAO (`ctx.fullscreenVao`) may be used for
 *      3-vertex fullscreen draws via gl_VertexID.
 */
class PostProcessPass {
 public:
  virtual ~PostProcessPass() noexcept = default;

  PostProcessPass(const PostProcessPass&) = delete;
  PostProcessPass& operator=(const PostProcessPass&) = delete;
  PostProcessPass(PostProcessPass&&) = delete;
  PostProcessPass& operator=(PostProcessPass&&) = delete;

  /*! Run the effect. Called once per frame if the pass is enabled. */
  virtual void execute(const PassContext& ctx) noexcept = 0;

  /*! Short human-readable name shown in debug UI. */
  [[nodiscard]] virtual std::string_view name() const noexcept = 0;

  /*! Hook to draw per-pass ImGui controls. Default no-op. */
  virtual void onDebugUi() noexcept {}

  [[nodiscard]] bool enabled() const noexcept { return enabled_; }
  void enabled(bool v) noexcept { enabled_ = v; }

 protected:
  PostProcessPass() noexcept = default;

 private:
  bool enabled_ = true;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_PASS_H_
