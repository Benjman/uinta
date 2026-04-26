#ifndef SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_POST_PROCESSOR_H_
#define SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_POST_PROCESSOR_H_

#include <array>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "uinta/component.h"
#include "uinta/engine/service_handle.h"
#include "uinta/gl.h"
#include "uinta/post_process/blit_helper.h"
#include "uinta/post_process/pass.h"
#include "uinta/post_process/render_target.h"
#include "uinta/post_process/resource_pool.h"
#include "uinta/types.h"
#include "uinta/vao.h"

namespace uinta {

class Engine;
class PostProcessor;

/*! PreRenderComponent that drives `PostProcessor::beginSimulation()` once per
 *  frame. Add to the Simulation-layer Scene that owns a PostProcessor. */
class PostProcessBeginComponent final : public PreRenderComponent {
 public:
  PostProcessBeginComponent(Engine* /*engine*/,
                            PostProcessor* processor) noexcept
      : processor_(processor) {}

  void update(time_t /*unused*/) noexcept override;

 private:
  PostProcessor* processor_;
};

/*! Scene-level orchestrator for the post-processing chain.
 *
 *  Owns:
 *    * the HDR simulation render target (color + depth),
 *    * two LDR/HDR ping-pong color targets used to carry intermediate pass
 *      results,
 *    * a shared `BlitHelper` (doubles as the "no passes enabled" fallback),
 *    * a shared empty VAO used by fullscreen triangle passes,
 *    * the mutable list of passes.
 *
 *  Integrates with the engine via three listeners:
 *    * `EngineEvent::ViewportSizeChange` -> reallocates render targets.
 *    * `EngineEvent::RenderLayerChange` -> binds the simulation target when
 *      entering the Simulation render layer's first scene's render is
 *      *imminent*, and runs the pass chain when transitioning out of
 *      Simulation.
 *    * `EngineEvent::RenderComplete` -> stores the current runtime for use
 *      by passes that need it.
 *
 *  Because `RenderLayerChange` only fires *between* scenes with different
 *  layers (not before the first Simulation scene), the PostProcessor also
 *  registers a `PreRenderComponent` on its owning scene to bind the
 *  simulation target at the start of Simulation rendering, after the engine's
 *  default-framebuffer clear.
 */
class PostProcessor {
 public:
  /*! Default dimensions used before the first `ViewportSizeChange` event. */
  static constexpr u32 kDefaultWidth = 1;
  static constexpr u32 kDefaultHeight = 1;

  explicit PostProcessor(Engine* engine) noexcept;

  ~PostProcessor() noexcept = default;

  PostProcessor(const PostProcessor&) = delete;
  PostProcessor& operator=(const PostProcessor&) = delete;
  PostProcessor(PostProcessor&&) = delete;
  PostProcessor& operator=(PostProcessor&&) = delete;

  /*! Add a pass of type `T` to the chain. Returns a non-owning pointer for
   *  configuration. Pass constructor must accept `(const OpenGLApi*, Args...)`.
   */
  template <typename T, typename... Args>
  T* addPass(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<PostProcessPass, T>,
                  "T must derive from PostProcessPass");
    auto pass = std::make_unique<T>(gl_, std::forward<Args>(args)...);
    T* raw = pass.get();
    passes_.push_back(std::move(pass));
    return raw;
  }

  /*! Remove a previously-added pass. Does nothing if `pass` is not present. */
  void removePass(const PostProcessPass* pass) noexcept;

  /*! Read-only access to the pass list for debug UI. */
  [[nodiscard]] const std::vector<std::unique_ptr<PostProcessPass>>& passes()
      const noexcept {
    return passes_;
  }
  [[nodiscard]] std::vector<std::unique_ptr<PostProcessPass>>&
  passes() noexcept {
    return passes_;
  }

  /*! Bind the simulation FBO and clear it. Meant to be called at the start of
   *  Simulation-layer rendering (typically via a PreRenderComponent). */
  void beginSimulation() noexcept;

  /*! Run the post-process chain. Meant to be called at the end of
   *  Simulation-layer rendering (typically via a RenderLayerChange listener
   *  firing when transitioning to UI). */
  void endSimulation() noexcept;

  /*! Trigger a resize; re-allocates all render targets. Called automatically
   *  by the viewport-size-change listener. */
  void resize(u32 width, u32 height) noexcept;

  [[nodiscard]] u32 width() const noexcept { return width_; }
  [[nodiscard]] u32 height() const noexcept { return height_; }
  [[nodiscard]] f32 near() const noexcept { return near_; }
  [[nodiscard]] f32 far() const noexcept { return far_; }
  [[nodiscard]] time_t runtime() const noexcept { return runtime_; }

  [[nodiscard]] const RenderTarget* sceneTarget() const noexcept {
    return sceneTarget_.get();
  }

  [[nodiscard]] const BlitHelper* blit() const noexcept { return &blit_; }

  [[nodiscard]] ResourcePool* pool() noexcept { return &pool_; }
  [[nodiscard]] const ResourcePool* pool() const noexcept { return &pool_; }

  /*! Override the view matrix supplied to passes. Typically populated
   *  automatically each frame from `CameraManager`; exposed for tests and
   *  scenes that use non-standard cameras. */
  void viewMatrix(const glm::mat4& m) noexcept { view_ = m; }

  [[nodiscard]] const glm::mat4& viewMatrix() const noexcept { return view_; }

  [[nodiscard]] const glm::mat4& projectionMatrix() const noexcept {
    return projection_;
  }

 private:
  ServiceHandle<PostProcessor> serviceHandle_;
  Engine* engine_;
  const OpenGLApi* gl_;

  u32 width_ = kDefaultWidth;
  u32 height_ = kDefaultHeight;
  f32 near_ = 0.1f;
  f32 far_ = 100.0f;
  time_t runtime_ = 0;
  glm::mat4 view_ = glm::mat4(1.0f);
  glm::mat4 projection_ = glm::mat4(1.0f);

  std::unique_ptr<RenderTarget> sceneTarget_;
  std::array<std::unique_ptr<RenderTarget>, 2> pingPong_;

  BlitHelper blit_;
  Vao fullscreenVao_;
  ResourcePool pool_;

  std::vector<std::unique_ptr<PostProcessPass>> passes_;

  bool initialized_ = false;

  void allocateTargets() noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_POST_PROCESS_POST_PROCESSOR_H_
