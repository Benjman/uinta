#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_

#include "uinta/engine_state.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/status.h"
#include "uinta/types.h"

namespace uinta {

class Platform;
class Scene;

class Engine {
 public:
  Engine(Platform*, Scene*,
         const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  ~Engine() noexcept = default;
  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;
  Engine(const Engine&&) noexcept = delete;
  Engine& operator=(const Engine&&) noexcept = delete;

  const Platform* platform() const noexcept { return platform_; }

  void onViewportChange(u32 width, u32 height) noexcept;

  const Status& status() const noexcept { return status_; }

  const EngineState& state() const noexcept { return state_; }

  Scene* scene() noexcept { return scene_; }

 private:
  struct FrameManager {
    explicit FrameManager(const Platform*);
    f32 nextFrame = 0;
    const u32 nextFrameAdvance;
  } frameManager_;
  Status status_;
  EngineState state_;

  const OpenGLApi* gl_;
  Platform* platform_;
  Scene* scene_;

  void newFrame() noexcept;
  void setCallbacks() noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_
