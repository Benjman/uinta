#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/engine/service_registry.h"
#include "uinta/input/input_system.h"
#include "uinta/localization/localization_system.h"
#include "uinta/lua/lua_runtime.h"
#include "uinta/platform.h"
#include "uinta/runtime_getter.h"
#include "uinta/scene/scene.h"
#include "uinta/scene/scene_system.h"
#include "uinta/types.h"

namespace uinta {

class AppConfig;
class ArgsProcessor;
struct OpenGLApi;
class ViewportManager;

struct EngineDependencies {
  Platform* platform;
  const OpenGLApi* gl;
  AppConfig* appConfig;
  const ArgsProcessor* args;
  ViewportManager* viewport;
};

class Engine : public RuntimeGetter {
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

  const Input* input() const noexcept { return inputSystem_.input(); }

  Input* input() noexcept { return inputSystem_.input(); }

  const Platform* platform() const noexcept { return platform_; }

  Platform* platform() noexcept { return platform_; }

  LuaRuntime* lua() noexcept { return &lua_; }

  const LuaRuntime* lua() const noexcept { return &lua_; }

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

  const SceneSystem* scenes() const noexcept { return &scenes_; }

  SceneSystem* scenes() noexcept { return &scenes_; }

  EngineState& state() noexcept { return state_; }

  const EngineState& state() const noexcept { return state_; }

 private:
  Platform* platform_;

  ServiceRegistry serviceRegistry_;
  EngineState state_;
  EngineDispatchers dispatchers_;
  LocalizationSystem localization_;
  InputSystem inputSystem_;
  SceneSystem scenes_;
  LuaRuntime lua_;

  template <EngineStage S>
  void advance() noexcept {
    auto delta = state_.updateStageDelta(S, runtime());
    scenes_.advance<S>(delta);
    lua_.dispatchStage(S, static_cast<f32>(delta));
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
