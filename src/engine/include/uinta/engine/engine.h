#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_

#include "uinta/args.h"
#include "uinta/engine/engine_events.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/engine/service_registry.h"
#include "uinta/gl.h"
#include "uinta/localization/locale.h"
#include "uinta/localization/localization_system.h"
#include "uinta/platform.h"
#include "uinta/runtime_getter.h"
#include "uinta/scene/scene.h"
#include "uinta/scene/scene_layer.h"
#include "uinta/status.h"
#include "uinta/types.h"
#include "uinta/utils/frame_manager.h"
#include "uinta/viewport/viewport_manager.h"

namespace uinta {

class AppConfig;

class Engine : public RuntimeGetter {
 public:
  struct Params final {
    Platform* platform;
    AppConfig* appConfig;
    const ArgsProcessor* args;
    const OpenGLApi* gl = OpenGLApiImpl::Instance();
    Locale locale = Locale::Unknown;
  };

  explicit Engine(Params) noexcept;

  ~Engine() noexcept = default;
  Engine(const Engine&) noexcept = delete;
  Engine& operator=(const Engine&) noexcept = delete;
  Engine(const Engine&&) noexcept = delete;
  Engine& operator=(const Engine&&) noexcept = delete;

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Scene, T>);
    sceneQueue_.push(std::make_unique<T>(this, std::forward<Args>(args)...));
    return reinterpret_cast<T*>(sceneQueue_.back().get());
  }

  EngineDispatchers* dispatchers() noexcept { return &dispatchers_; }

  const FrameManager& frameManager() const noexcept { return frame_; }

  FrameManager& frameManager() noexcept { return frame_; }

  const OpenGLApi* gl() const noexcept { return gl_; }

  const Platform* platform() const noexcept { return platform_; }

  Platform* platform() noexcept { return platform_; }

  template <typename T>
  void registerService(T* service) noexcept {
    services_.registerService<T>(service);
    dispatchers_.dispatch<EngineEvent::ServiceRegistered>(
        ServiceRegistered{.type = std::type_index(typeid(T)),
                          .service = static_cast<void*>(service)});
  }

  template <typename T>
  void unregisterService() noexcept {
    services_.unregisterService<T>();
    dispatchers_.dispatch<EngineEvent::ServiceUnregistered>(
        ServiceUnregistered{std::type_index(typeid(T))});
  }

  template <typename T>
  T* service() noexcept {
    return services_.service<T>();
  }

  template <typename T>
  const T* service() const noexcept {
    return services_.service<T>();
  }

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

  void setStatusError(const Status& status) noexcept {
    assert(!status.ok());
    if (status_.ok()) {
      status_ = status;
    }
  }

 private:
  EngineState state_;
  EngineDispatchers dispatchers_;
  ServiceRegistry services_;
  ViewportManager viewport;
  FrameManager frame_;
  LocalizationSystem localization_;
  std::queue<std::unique_ptr<Scene>> sceneQueue_;
  std::vector<Scene*> renderOrder_;
  Status status_;

  const OpenGLApi* gl_;
  Platform* platform_;

  time_t getRuntime() noexcept {
    auto status = platform_->runtime();
    if (status.ok()) {
      return status.value();
    }
    status_ = status.status();
    return state_.runtime();
  }

  void preTick(Scene*, time_t delta) noexcept;
  void tick(Scene*, time_t delta) noexcept;
  void postTick(Scene*, time_t delta) noexcept;
  void preRender(Scene*, time_t delta) noexcept;
  void render(Scene*, time_t delta) noexcept;
  void postRender(Scene*, time_t delta) noexcept;

  template <EngineStage S>
  void advance(Scene* scene) noexcept {
    auto delta = state_.updateStageDelta(S, getRuntime());

    if (scene) {
      scene->updateComponents<S>(delta);

      if constexpr (S == EngineStage::PreTick) {
        preTick(scene, delta);
      } else if constexpr (S == EngineStage::Tick) {
        tick(scene, delta);
      } else if constexpr (S == EngineStage::PostTick) {
        postTick(scene, delta);
      } else if constexpr (S == EngineStage::PreRender) {
        preRender(scene, delta);
      } else if constexpr (S == EngineStage::Render) {
        auto layer = SceneLayers.front();
        assert(layer == SceneLayer::Simulation);
        std::for_each(renderOrder_.begin(), renderOrder_.end(),
                      [this, &layer, delta](auto* scene) {
                        if (layer != scene->layer()) {
                          layer = scene->layer();
                          dispatchers_.dispatch<EngineEvent::RenderLayerChange>(
                              RenderLayerChange(layer));
                        }
                        render(scene, delta);
                      });
      } else if constexpr (S == EngineStage::PostRender) {
        postRender(scene, delta);
      }
    }
  }

  void updateRenderOrder() noexcept;

  void registerSceneListeners(Scene*) noexcept;
};

using EngineParams = Engine::Params;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_H_
