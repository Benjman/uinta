#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include <type_traits>
#include <utility>

#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/flags.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/status.h"
#include "uinta/platform.h"
#include "uinta/runtime_getter.h"
#include "uinta/types.h"
#include "uinta/utils/frame_manager.h"

namespace uinta {

class Platform;

class Engine : public RuntimeGetter {
 public:
  struct Flags final {
    using value_type = u8;
    // `FixedTickRate` will run the tick loop only once per frame.
    static constexpr value_type FixedTickRateMask = 1 << 0;

    bool isFixedTickRate() const noexcept { return flags_ & FixedTickRateMask; }
    void isFixedTickRate(bool v) noexcept {
      flags_ &= ~FixedTickRateMask;
      if (v) flags_ |= FixedTickRateMask;
    }

    FlagsOperations(FixedTickRateMask);

   private:
    value_type flags_;
  };

  explicit Engine(Platform*, const OpenGLApi*) noexcept;

  ~Engine() noexcept = default;
  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;
  Engine(const Engine&&) noexcept = delete;
  Engine& operator=(const Engine&&) noexcept = delete;

  template <EngineEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  EngineDispatchers* dispatchers() noexcept { return &dispatchers_; }

  Flags flags() const noexcept { return flags_; }

  void flags(Flags flags) noexcept { flags_ = flags; }

  const FrameManager& frameManager() const noexcept { return frame_; }

  FrameManager& frameManager() noexcept { return frame_; }

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
  EngineDispatchers dispatchers_;
  FrameManager frame_;
  Status status_;
  Flags flags_;

  const OpenGLApi* gl_;
  Platform* platform_;

  template <EngineEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    dispatchers_.template dispatch<E>(std::forward<Args>(args)...);
  }

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
