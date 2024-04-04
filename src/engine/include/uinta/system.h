#ifndef SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_

#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "absl/synchronization/mutex.h"
#include "uinta/engine/engine_stage.h"

namespace uinta {

class System {
  friend class SystemManager;

 public:
  virtual ~System() noexcept = default;

  virtual void onPostRender(time_t) noexcept {}
  virtual void onPostTick(time_t) noexcept {}
  virtual void onPreRender(time_t) noexcept {}
  virtual void onPreTick(time_t) noexcept {}
  virtual void onRender(time_t) noexcept {}
  virtual void onTick(time_t) noexcept {}

 protected:
  System() noexcept = default;
};

class SystemManager {
 public:
  enum class Type : u8 {
    PostRender,
    PostTick,
    PreRender,
    PreTick,
    Render,
    Tick,
  };

  template <typename T, typename... Args>
  T* add(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<System, T>);
    absl::MutexLock lock(&mtx_);
    return reinterpret_cast<T*>(
        systems_
            .emplace_back(std::unique_ptr<T>(
                std::make_unique<T>(std::forward<Args>(args)...)))
            .get());
  }

  template <typename T>
  void remove(const T* system) noexcept {
    // TODO: This needs unit tests
    static_assert(std::is_base_of_v<System, T>);
    absl::MutexLock lock(&mtx_);
    if (auto itr = std::find_if(systems_.begin(), systems_.end(),
                                [system](const std::unique_ptr<System>& ptr) {
                                  return ptr.get() == system;
                                });
        itr != systems_.end()) {
      systems_.erase(itr);
    }
  }

  template <EngineStage S>
  void update(time_t delta) noexcept {
    constexpr std::array<SystemManager::Type, 7> EngineStageToType = {
        SystemManager::Type::PreTick,  SystemManager::Type::Tick,
        SystemManager::Type::PostTick, SystemManager::Type::PreRender,
        SystemManager::Type::Render,   SystemManager::Type::PostRender,
    };
    update<EngineStageToType[static_cast<u8>(S)]>(delta);
  }

  template <SystemManager::Type T>
  void update(time_t delta) noexcept {
    constexpr auto runtimeAction = []() {
      if constexpr (T == Type::PreTick) {
        return [](System* system, time_t delta) noexcept {
          system->onPreTick(delta);
        };
      } else if constexpr (T == Type::Tick) {
        return [](System* system, time_t delta) noexcept {
          system->onTick(delta);
        };
      } else if constexpr (T == Type::PostTick) {
        return [](System* system, time_t delta) noexcept {
          system->onPostTick(delta);
        };
      } else if constexpr (T == Type::PreRender) {
        return [](System* system, time_t delta) noexcept {
          system->onPreRender(delta);
        };
      } else if constexpr (T == Type::Render) {
        return [](System* system, time_t delta) noexcept {
          system->onRender(delta);
        };
      } else if constexpr (T == Type::PostRender) {
        return [](System* system, time_t delta) noexcept {
          system->onPostRender(delta);
        };
      }
    }();

    std::for_each(systems_.begin(), systems_.end(),
                  [delta, runtimeAction](auto& system) {
                    runtimeAction(system.get(), delta);
                  });
  }

  template <typename T>
  std::optional<T*> find() const noexcept {
    static_assert(std::is_base_of_v<System, T>);
    if (auto itr = std::find_if(systems_.begin(), systems_.end(),
                                [](const auto& system) {
                                  return dynamic_cast<const T*>(system.get());
                                });
        itr != systems_.end()) {
      return reinterpret_cast<T*>(itr->get());
    }
    return std::nullopt;
  }

  auto size() const noexcept { return systems_.size(); }

 private:
  absl::Mutex mtx_;
  std::vector<std::unique_ptr<System>> systems_;
};

using SystemType = SystemManager::Type;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SYSTEM_H_
