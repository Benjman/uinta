#ifndef SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "uinta/component.h"

namespace uinta {

class System : public ComponentManager {
  friend class SystemManager;

 public:
  virtual void onNewFrame(const EngineState&) noexcept {}
  virtual void onPostRender(const EngineState&) noexcept {}
  virtual void onPostTick(const EngineState&) noexcept {}
  virtual void onPreRender(const EngineState&) noexcept {}
  virtual void onPreTick(const EngineState&) noexcept {}
  virtual void onRender(const EngineState&) noexcept {}
  virtual void onTick(const EngineState&) noexcept {}

 protected:
  System() noexcept = default;
};

class RenderSystem : public System {};

class SystemManager {
  using SystemList = std::vector<std::unique_ptr<System>>;

 public:
  template <typename T, typename... Args>
  T* addSystem(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<System, T>, "Must be derived from System.");
    return reinterpret_cast<T*>(
        systems_
            .emplace_back(std::unique_ptr<T>(
                std::make_unique<T>(std::forward<Args>(args)...)))
            .get());
  }

  template <EngineStage S>
  void updateSystems(const EngineState& state) noexcept {
    constexpr auto newFrameAction = [](auto& system,
                                       const auto& state) noexcept {
      system->onNewFrame(state);
    };
    constexpr auto preTickAction = [](auto& system,
                                      const auto& state) noexcept {
      system->onPreTick(state);
    };
    constexpr auto tickAction = [](auto& system, const auto& state) noexcept {
      system->onTick(state);
    };
    constexpr auto postTickAction = [](auto& system,
                                       const auto& state) noexcept {
      system->onPostTick(state);
    };
    constexpr auto preRenderAction = [](auto& system,
                                        const auto& state) noexcept {
      system->onPreRender(state);
    };
    constexpr auto renderAction = [](auto& system, const auto& state) noexcept {
      system->onRender(state);
    };
    constexpr auto postRenderAction = [](auto& system,
                                         const auto& state) noexcept {
      system->onPostRender(state);
    };

    constexpr auto runtimeAction = []() {
      if constexpr (S == EngineStage::NewFrame)
        return newFrameAction;
      else if constexpr (S == EngineStage::PreTick)
        return preTickAction;
      else if constexpr (S == EngineStage::Tick)
        return tickAction;
      else if constexpr (S == EngineStage::PostTick)
        return postTickAction;
      else if constexpr (S == EngineStage::PreRender)
        return preRenderAction;
      else if constexpr (S == EngineStage::Render)
        return renderAction;
      else if constexpr (S == EngineStage::PostRender)
        return postRenderAction;
    }();

    std::for_each(systems_.begin(), systems_.end(),
                  [&state, runtimeAction](auto& system) {
                    system->template updateComponents<S>(state);
                    runtimeAction(system, state);
                  });
  }

 private:
  SystemList systems_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_
