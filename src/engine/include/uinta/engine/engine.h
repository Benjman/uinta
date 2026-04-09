#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/engine/service_registry.h"
#include "uinta/platform.h"
#include "uinta/runtime_getter.h"
#include "uinta/types.h"

namespace uinta {

class AppConfig;
class ArgsProcessor;
struct OpenGLApi;

struct EngineDependencies {
  Platform* platform;
  const OpenGLApi* gl;
  AppConfig* appConfig;
  const ArgsProcessor* args;
};

class Engine : public RuntimeGetter {
 public:
  explicit Engine(const EngineDependencies& deps) noexcept;

  ~Engine() noexcept = default;
  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;
  Engine(const Engine&&) noexcept = delete;
  Engine& operator=(const Engine&&) noexcept = delete;

  EngineDispatchers* dispatchers() noexcept { return &dispatchers_; }

  const Platform* platform() const noexcept { return platform_; }

  Platform* platform() noexcept { return platform_; }

  template <typename T>
  void registerService(T* service) noexcept {
    serviceRegistry_.registerService<T>(service);
    dispatchers_.dispatch<EngineEvent::ServiceRegistered>(
        ServiceRegistered{.type = std::type_index(typeid(T)),
                          .service = const_cast<void*>(static_cast<const void*>(service)),
                          .isConst = std::is_const_v<T>});
  }

  template <typename T>
  void unregisterService() noexcept {
    serviceRegistry_.unregisterService<T>();
    dispatchers_.dispatch<EngineEvent::ServiceUnregistered>(ServiceUnregistered{std::type_index(typeid(T))});
  }

  template <typename T>
  T* service() noexcept {
    return serviceRegistry_.service<T>();
  }

  template <typename T>
  const T* service() const noexcept {
    return serviceRegistry_.service<T>();
  }

  time_t runtime() const noexcept override { return platform_->runtime().value_or(state_.runtime()); }

  void run() noexcept;

  EngineState& state() noexcept { return state_; }

  const EngineState& state() const noexcept { return state_; }

 private:
  Platform* platform_;

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
