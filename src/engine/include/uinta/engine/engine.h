#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include <memory>
#include <queue>
#include <type_traits>
#include <utility>

#include "uinta/component.h"
#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/flags.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/status.h"
#include "uinta/platform.h"
#include "uinta/runtime_getter.h"
#include "uinta/scene.h"
#include "uinta/system.h"
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

  template <typename T, typename... Args>
  T* addComponent(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Component, T>);
    return components_.add<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  void removeComponent(T* component) noexcept {
    static_assert(std::is_base_of_v<Component, T>);
    components_.remove(component);
  }

  template <EngineEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  void addScene(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Scene, T>);
    sceneQueue_.push(std::make_unique<T>(std::forward<Args>(args)...));
  }

  template <typename T, typename... Args>
  T* addSystem(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<System, T>);
    return systems_.add<T>(std::forward<Args>(args)...);
  }

  const ComponentManager* components() const noexcept { return &components_; }

  ComponentManager* components() noexcept { return &components_; }

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

  const auto* scenes() const noexcept { return &sceneQueue_; }

  auto* scenes() noexcept { return &sceneQueue_; }

  EngineState& state() noexcept { return state_; }

  const EngineState& state() const noexcept { return state_; }

  const Status& status() const noexcept { return status_; }

  Status& status() noexcept { return status_; }

  const SystemManager* systems() const noexcept { return &systems_; }

  SystemManager* systems() noexcept { return &systems_; }

 private:
  ComponentManager components_;
  EngineState state_;
  EngineDispatchers dispatchers_;
  FrameManager frame_;
  std::queue<std::unique_ptr<Scene>> sceneQueue_;
  Status status_;
  SystemManager systems_;
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

  void preTick(Scene*) noexcept;
  void tick(Scene*) noexcept;
  void postTick(Scene*) noexcept;
  void preRender(Scene*) noexcept;
  void render(Scene*) noexcept;
  void postRender(Scene*) noexcept;

  template <EngineStage S>
  void advance(Scene* scene) noexcept {
    if (!scene) return;
    scene->updateComponents<S>(state_);
    scene->updateSystems<S>(state_);
    if constexpr (S == EngineStage::PreTick) {
      preTick(scene);
    } else if constexpr (S == EngineStage::Tick) {
      tick(scene);
    } else if constexpr (S == EngineStage::PostTick) {
      postTick(scene);
    } else if constexpr (S == EngineStage::PreRender) {
      preRender(scene);
    } else if constexpr (S == EngineStage::Render) {
      render(scene);
    } else if constexpr (S == EngineStage::PostRender) {
      postRender(scene);
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
