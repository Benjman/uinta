#ifndef SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_

#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/lib/absl/synchronization.h"

namespace uinta {

class System {
  friend class SystemManager;

 public:
  virtual ~System() noexcept = default;

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

class SystemManager {
 public:
  enum class Type : u8 {
    NewFrame,
    PostRender,
    PostTick,
    PreRender,
    PreTick,
    Render,
    Tick,
  };

  template <typename T, typename... Args>
  T* add(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<System, T>, "Must be derived from System.");
    MutexLock lock(&mtx_);
    return reinterpret_cast<T*>(
        systems_
            .emplace_back(std::unique_ptr<T>(
                std::make_unique<T>(std::forward<Args>(args)...)))
            .get());
  }

  template <EngineStage S>
  void update(const EngineState& state) noexcept {
    constexpr std::array<SystemManager::Type, 7> EngineStageToType = {
        SystemManager::Type::PreTick,  SystemManager::Type::Tick,
        SystemManager::Type::PostTick, SystemManager::Type::PreRender,
        SystemManager::Type::Render,   SystemManager::Type::PostRender,
    };
    update<EngineStageToType[static_cast<u8>(S)]>(state);
  }

  template <SystemManager::Type T>
  void update(const EngineState& state) noexcept {
    constexpr auto runtimeAction = []() {
      if constexpr (T == Type::NewFrame) {
        return [](auto* system, const auto& state) noexcept {
          system->onNewFrame(state);
        };
      } else if constexpr (T == Type::PreTick) {
        return [](auto* system, const auto& state) noexcept {
          system->onPreTick(state);
        };
      } else if constexpr (T == Type::Tick) {
        return [](auto* system, const auto& state) noexcept {
          system->onTick(state);
        };
      } else if constexpr (T == Type::PostTick) {
        return [](auto* system, const auto& state) noexcept {
          system->onPostTick(state);
        };
      } else if constexpr (T == Type::PreRender) {
        return [](auto* system, const auto& state) noexcept {
          system->onPreRender(state);
        };
      } else if constexpr (T == Type::Render) {
        return [](auto* system, const auto& state) noexcept {
          system->onRender(state);
        };
      } else if constexpr (T == Type::PostRender) {
        return [](auto* system, const auto& state) noexcept {
          system->onPostRender(state);
        };
      }
    }();

    std::for_each(systems_.begin(), systems_.end(),
                  [&state, runtimeAction](auto& system) {
                    runtimeAction(system.get(), state);
                  });
  }

  template <typename T>
  std::optional<T*> find() const noexcept {
    if (auto itr = std::find_if(systems_.begin(), systems_.end(),
                                [](const auto& system) {
                                  return dynamic_cast<const T*>(system.get());
                                });
        itr != systems_.end())
      return reinterpret_cast<T*>(itr->get());
    return std::nullopt;
  }

  auto size() const noexcept { return systems_.size(); }

 private:
  Mutex mtx_;
  std::vector<std::unique_ptr<System>> systems_;
};

using SystemType = SystemManager::Type;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_
