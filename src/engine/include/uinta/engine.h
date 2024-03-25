#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_

#include "uinta/component.h"
#include "uinta/engine_state.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/status.h"
#include "uinta/types.h"

namespace uinta {

class Platform;

class Engine {
 public:
  explicit Engine(Platform*,
                  const OpenGLApi* = OpenGLApiImpl::GetInstance()) noexcept;

  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;

  Engine(Engine&&) noexcept;
  Engine& operator=(Engine&&) noexcept;

  const Platform* platform() const noexcept { return platform_; }

  void onViewportChange(u32 width, u32 height) noexcept;

  const Status& status() const noexcept { return status_; }

  const EngineState& state() const noexcept { return state_; }

  ComponentList& components() noexcept { return components_; }

 private:
  Status status_;
  EngineState state_;
  ComponentList components_;

  const OpenGLApi* gl_;
  Platform* platform_;

  void newFrame() noexcept;
  void setCallbacks() noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_H_
