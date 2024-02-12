#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/flags.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/status.h"
#include "uinta/platform.h"
#include "uinta/runtime_getter.h"
#include "uinta/types.h"

namespace uinta {

class Platform;

class Engine : public RuntimeGetter {
 public:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(0);

   private:
    value_type flags_;
  };

  explicit Engine(Platform*, const OpenGLApi*) noexcept;

  ~Engine() noexcept = default;
  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;
  Engine(const Engine&&) noexcept = delete;
  Engine& operator=(const Engine&&) noexcept = delete;

  Flags flags() const noexcept { return flags_; }

  void flags(Flags flags) noexcept { flags_ = flags; }

  const OpenGLApi* gl() const noexcept { return gl_; }

  const Platform* platform() const noexcept { return platform_; }

  Platform* platform() noexcept { return platform_; }

  time_t runtime() const noexcept override { return state_.runtime(); }

  void run() noexcept;

  EngineState& state() noexcept { return state_; }

  const EngineState& state() const noexcept { return state_; }

  const Status& status() const noexcept { return status_; }

  Status& status() noexcept { return status_; }

 private:
  EngineState state_;
  Status status_;
  Flags flags_;

  const OpenGLApi* gl_;
  Platform* platform_;

  time_t getRuntime() noexcept {
    if (auto status = platform_->runtime(); status.ok()) {
      return status.value();
    } else {
      status_ = status.status();
      return state_.runtime();
    }
  }

  void preTick() noexcept;
  void preRender() noexcept;
  void tick() noexcept;
  void render() noexcept;
  void postTick() noexcept;
  void postRender() noexcept;

  template <EngineStage S>
  void advance() noexcept {
    if constexpr (S == EngineStage::PreTick) {
      preTick();
    } else if constexpr (S == EngineStage::Tick) {
      tick();
    } else if constexpr (S == EngineStage::PostTick) {
      postTick();
    } else if constexpr (S == EngineStage::PreRender) {
      preRender();
    } else if constexpr (S == EngineStage::Render) {
      render();
    } else if constexpr (S == EngineStage::PostRender) {
      postRender();
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
