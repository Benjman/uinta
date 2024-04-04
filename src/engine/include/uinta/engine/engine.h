#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include <optional>
#include <utility>

#include "uinta/component.h"
#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/file.h"
#include "uinta/gl.h"
#include "uinta/platform.h"
#include "uinta/reflect.h"
#include "uinta/runtime_getter.h"
#include "uinta/shader.h"
#include "uinta/status.h"
#include "uinta/system.h"
#include "uinta/texture.h"
#include "uinta/types.h"
#include "uinta/uniform.h"
#include "uinta/utils/frame_manager.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Engine : public RuntimeGetter {
 public:
  struct Params final {
    Platform* platform;
    AppConfig* appConfig;
    const OpenGLApi* gl = OpenGLApiImpl::Instance();
    FileSystem* fileSystem = FileSystemImpl::Instance();
  };

  explicit Engine(Params) noexcept;

  ~Engine() noexcept = default;
  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;
  Engine(const Engine&&) noexcept = delete;
  Engine& operator=(const Engine&&) noexcept = delete;

  AppConfig* appConfig() const noexcept { return appConfig_; }

  AppConfig* appConfig() noexcept { return appConfig_; }

  const ComponentManager* components() const noexcept { return &components_; }

  ComponentManager* components() noexcept { return &components_; }

  template <typename T, typename... Args>
  T* addComponent(Args&&... args) noexcept {
    return components_.add<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  void removeComponent(T* component) noexcept {
    components_.remove(component);
  }

  template <typename T, typename... Args>
  T* addSystem(Args&&... args) noexcept {
    return systems_.add<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  void removeSystem(T* system) noexcept {
    systems_.remove(system);
  }

  template <typename T>
  std::optional<T*> findSystem() const noexcept {
    if (std::optional<T*> system = systems_.find<T>(); system.has_value()) {
      return system;
    } else {
      return std::nullopt;
    }
  }

  template <typename T>
  T* findSystemOrThrow() const {
    if (std::optional<T*> system = findSystem<T>(); system != std::nullopt) {
      return system.value();
    } else {
      throw std::runtime_error(
          absl::StrFormat("Required system not found: '%s'",
                          demangleTypeName(typeid(T).name())));
    }
  }

  EngineDispatchers* dispatchers() noexcept { return &dispatchers_; }

  const FileSystem* fileSystem() const noexcept { return fileSystem_; }

  FileSystem* fileSystem() noexcept { return fileSystem_; }

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

  void setStatusError(Status status) noexcept {
    assert(!status.ok());
    if (status_.ok()) {
      status_ = status;
    }
  }

  const SystemManager* systems() const noexcept { return &systems_; }

  SystemManager* systems() noexcept { return &systems_; }

 private:
  ComponentManager components_;
  EngineState state_;
  EngineDispatchers dispatchers_;
  FrameManager frame_;
  Status status_;
  SystemManager systems_;

  AppConfig* appConfig_;
  FileSystem* fileSystem_;
  const OpenGLApi* gl_;
  Platform* platform_;

  Shader shader_;
  UniformMatrix4fv uProjection_;
  Uniform4fv uColor_;
  Vao vao_;
  Vbo vbo_;
  Texture texture_;

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
    auto delta = state_.delta();

    components_.update<S>(delta);
    systems_.update<S>(delta);

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

using EngineParams = Engine::Params;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
