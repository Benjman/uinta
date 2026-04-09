#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/engine/service_registry.h"
#include "uinta/localization/localization_system.h"
#include "uinta/platform.h"
#include "uinta/scene/scene.h"
#include "uinta/scene/scene_system.h"

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

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    return scenes_.addScene<T>(std::forward<Args>(args)...);
  }

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

  const SceneSystem* scenes() const noexcept { return &scenes_; }

  SceneSystem* scenes() noexcept { return &scenes_; }

  EngineState& state() noexcept { return state_; }

  const EngineState& state() const noexcept { return state_; }

 private:
  ServiceRegistry serviceRegistry_;
  EngineState state_;
  EngineDispatchers dispatchers_;
  LocalizationSystem localization_;
  SceneSystem scenes_;

  template <EngineStage S>
  void advance() noexcept {
    auto delta = state_.updateStageDelta(S, state_.runtime());
    scenes_.advance<S>(delta);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
