#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/engine/service_registry.h"
#include "uinta/platform.h"

namespace uinta {

class AppConfig;
class ArgsProcessor;
struct OpenGLApi;
class Platform;

struct EngineDependencies {
  Platform* platform;
  const OpenGLApi* gl;
  AppConfig* appConfig;
  const ArgsProcessor* args;
};

class Engine {
 public:
  explicit Engine(const EngineDependencies& deps) noexcept;

  ~Engine() noexcept = default;
  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;
  Engine(const Engine&&) noexcept = delete;
  Engine& operator=(const Engine&&) noexcept = delete;

  EngineDispatchers* dispatchers() noexcept { return &dispatchers_; }

  template <typename T>
  void registerService(T* service) noexcept {
    registerService(std::type_index(typeid(T)), const_cast<void*>(static_cast<const void*>(service)),
                    std::is_const_v<T>);
  }

  void registerService(std::type_index type, void* service, bool isConst) noexcept {
    serviceRegistry_.registerService(type, service, isConst);
    dispatchers_.dispatch<EngineEvent::ServiceRegistered>(
        ServiceRegistered{.type = type, .service = service, .isConst = isConst});
  }

  template <typename T>
  void unregisterService() noexcept {
    unregisterService(std::type_index(typeid(T)));
  }

  void unregisterService(std::type_index type) noexcept {
    serviceRegistry_.unregisterService(type);
    dispatchers_.dispatch<EngineEvent::ServiceUnregistered>(ServiceUnregistered{type});
  }

  template <typename T>
  T* service() noexcept {
    return serviceRegistry_.service<T>();
  }

  template <typename T>
  const T* service() const noexcept {
    return serviceRegistry_.service<T>();
  }

  void run() noexcept;

  EngineState& state() noexcept { return state_; }

  const EngineState& state() const noexcept { return state_; }

 private:
  ServiceRegistry serviceRegistry_;
  EngineState state_;
  EngineDispatchers dispatchers_;

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
