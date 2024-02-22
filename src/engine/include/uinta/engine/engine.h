#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include <memory>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

#include "uinta/component.h"
#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/file.h"
#include "uinta/gl.h"
#include "uinta/input/input.h"
#include "uinta/platform.h"
#include "uinta/reflect.h"
#include "uinta/runtime_getter.h"
#include "uinta/scene/scene.h"
#include "uinta/scene/scene_layer.h"
#include "uinta/status.h"
#include "uinta/system.h"
#include "uinta/types.h"
#include "uinta/utils/frame_manager.h"

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
  T* addScene(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Scene, T>);
    sceneQueue_.push(std::make_unique<T>(this, std::forward<Args>(args)...));
    return reinterpret_cast<T*>(sceneQueue_.back().get());
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
      if (sceneQueue_.empty()) {
        return std::nullopt;
      } else {
        return sceneQueue_.front()->findSystem<T>();
      }
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

  const Input* input() const noexcept { return &input_; }

  Input* input() noexcept { return &input_; }

  const Platform* platform() const noexcept { return platform_; }

  Platform* platform() noexcept { return platform_; }

  time_t runtime() const noexcept override { return state_.runtime(); }

  void run() noexcept;

  const std::queue<std::unique_ptr<Scene>>* scenes() const noexcept {
    return &sceneQueue_;
  }

  std::queue<std::unique_ptr<Scene>>* scenes() noexcept { return &sceneQueue_; }

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
  Input input_;
  std::queue<std::unique_ptr<Scene>> sceneQueue_;
  std::vector<Scene*> renderOrder_;
  Status status_;
  SystemManager systems_;

  AppConfig* appConfig_;
  FileSystem* fileSystem_;
  const OpenGLApi* gl_;
  Platform* platform_;

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
    auto delta = state_.delta();

    components_.update<S>(delta);
    systems_.update<S>(delta);

    if (scene) {
      scene->updateComponents<S>(delta);
      scene->updateSystems<S>(delta);

      if constexpr (S == EngineStage::PreTick) {
        preTick(scene);
      } else if constexpr (S == EngineStage::Tick) {
        tick(scene);
      } else if constexpr (S == EngineStage::PostTick) {
        postTick(scene);
      } else if constexpr (S == EngineStage::PreRender) {
        preRender(scene);
      } else if constexpr (S == EngineStage::Render) {
        auto layer = SceneLayers.front();
        assert(layer == SceneLayer::Simulation);
        std::for_each(renderOrder_.begin(), renderOrder_.end(),
                      [this, &layer](auto* scene) {
                        if (layer != scene->layer()) {
                          layer = scene->layer();
                          dispatchers_.dispatch<EngineEvent::RenderLayerChange>(
                              RenderLayerChange(layer));
                        }
                        render(scene);
                      });
      } else if constexpr (S == EngineStage::PostRender) {
        postRender(scene);
      }
    }
  }

  void updateRenderOrder() noexcept;

  void registerSceneListeners(Scene*) noexcept;
};

using EngineParams = Engine::Params;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
