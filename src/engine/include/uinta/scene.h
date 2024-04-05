#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_H_

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/log.h"
#include "uinta/component.h"
#include "uinta/engine/engine_state.h"
#include "uinta/flags.h"
#include "uinta/lib/absl/status.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/lib/absl/synchronization.h"
#include "uinta/runtime_getter.h"
#include "uinta/system.h"
#include "uinta/types.h"

namespace uinta {

class Engine;
class EngineState;
class Scene;

enum class SceneEventType {
  StateChange,
  LayerChange,
};

template <SceneEventType T>
struct SceneEvent {
  explicit SceneEvent(Scene* scene) noexcept : scene(scene) {}

  SceneEventType type() const noexcept { return T; }

  Scene* scene;
};

class Scene : public RuntimeGetter {
 public:
  class SceneStack {
    using Stack = std::vector<std::unique_ptr<Scene>>;

   public:
    template <typename T>
    static std::optional<T*> Find(const Scene* scene,
                                  bool seekRoot = true) noexcept {
      static_assert(std::is_base_of<Scene, T>::value,
                    "T must be a derived class of Scene");

      if (auto* casted = dynamic_cast<const T*>(scene))
        return const_cast<T*>(casted);

      const auto* root = scene;
      while (seekRoot && root->parent_) {
        if (auto* casted = dynamic_cast<const T*>(root))
          return const_cast<T*>(casted);
        root = scene->parent_;
      }

      for (const auto& scene : root->sceneStack_.stack_) {
        if (auto* casted = dynamic_cast<T*>(scene.get()))
          return const_cast<T*>(casted);
      }

      for (const auto& scene : root->sceneStack_.stack_) {
        if (auto result = Find<T>(scene.get())) return result;
      }

      return std::nullopt;
    }

    template <typename T, typename... Args>
    T* add(Scene* parent, Engine* engine, Args&&... args) noexcept {
      assert((parent || engine) && "`Scene*` or `Engine*` is required.");
      auto scene =
          std::make_unique<T>(parent, engine, std::forward<Args>(args)...);
      if (State::Error == scene->state()) {
        logWarning(StrFormat("Failed to add scene %s.", scene->name()).c_str());
        return nullptr;
      }
      MutexLock lock(&mtx_);
      auto itr = stack_.begin();
      for (; itr != stack_.end(); ++itr)
        if (itr->get()->layer() > scene->layer()) break;
      itr = stack_.insert(itr, std::move(scene));
      // TODO: Dispatch scene added.
      return reinterpret_cast<T*>(itr->get());
    }

    bool contains(const Scene* scene) noexcept {
      return std::find_if(stack_.begin(), stack_.end(),
                          [&scene](const auto& s) {
                            return scene == s.get();
                          }) != stack_.end();
    }

    void remove(const Scene* scene) noexcept {
      assert(scene && "Scene cannot be null.");
      MutexLock lock(&mtx_);
      auto end =
          std::remove_if(stack_.begin(), stack_.end(),
                         [&scene](const auto& s) { return s.get() == scene; });
      stack_.erase(end, stack_.end());
    }

    Stack& stack() noexcept { return stack_; }

    auto begin() const noexcept { return stack_.begin(); }

    auto end() const noexcept { return stack_.end(); }

    auto begin() noexcept { return stack_.begin(); }

    auto end() noexcept { return stack_.end(); }

    auto size() const noexcept { return stack_.size(); }

    void clear() noexcept { stack_.clear(); }

   private:
    Stack stack_;
    Mutex mtx_;
  };

  enum class Layer : u8 {
    Simulation,
    UI,
    Debug,
  };

  static constexpr std::array<Layer, 3> Layers = {
      Layer::Simulation,
      Layer::UI,
      Layer::Debug,
  };

  enum class State : u8 {
    Running,
    Pause,
    Error,
    Complete,
  };

  static constexpr std::array<State, 4> States = {
      State::Running,
      State::Pause,
      State::Error,
      State::Complete,
  };

  static constexpr std::array<Layer, 3> RenderOrder = {
      Layer::Simulation,
      Layer::UI,
      Layer::Debug,
  };

  struct Flags final {
    using value_type = u8;

    struct Shifts final {
      static constexpr value_type Layer = 0;
      static constexpr value_type State = 3;
    };

    struct Masks final {
      static constexpr value_type Layer = 0x7 << Shifts::Layer;
      static constexpr value_type State = 0x7 << Shifts::State;
    };

    AtomicFlagsOperations(0);

    bool isComplete() const noexcept { return state() == State::Complete; }

    bool isError() const noexcept { return state() == State::Error; }

    bool isPaused() const noexcept { return state() == State::Pause; }

    bool isRunning() const noexcept { return state() == State::Running; }

    bool isRendering() const noexcept { return isRunning(); }

    bool isTicking() const noexcept { return isRunning(); }

    Layer layer() const noexcept {
      return static_cast<Layer>((flags_ & Masks::Layer) >> Shifts::Layer);
    }

    Layer layer(Layer l) noexcept {
      flags_ &= ~Masks::Layer;
      flags_ |= static_cast<value_type>(l) << Shifts::Layer;
      return layer();
    }

    State state() const noexcept {
      return static_cast<State>((flags_ & Masks::State) >> Shifts::State);
    }

    State state(State s) noexcept {
      flags_ &= ~Masks::State;
      flags_ |= static_cast<value_type>(s) << Shifts::State;
      return state();
    }

   private:
    std::atomic<value_type> flags_;
  };

  template <typename T>
  static std::optional<T*> FindScene(const Scene* scene) noexcept {
    return SceneStack::Find<T>(scene);
  }

  Scene(Scene* parent = nullptr, Engine* engine = nullptr,
        Layer = Layer::Debug) noexcept;

  virtual ~Scene() noexcept {
    parent_ = nullptr;
    engine_ = nullptr;
    LOG(INFO) << "Destructed.";
  }

  Scene(const Scene&) noexcept = delete;
  Scene& operator=(const Scene&) noexcept = delete;
  Scene(Scene&&) noexcept = delete;
  Scene& operator=(Scene&&) noexcept = delete;

  virtual constexpr std::string name() const noexcept = 0;

  virtual void preTick(const EngineState&) noexcept {}
  virtual void tick(const EngineState&) noexcept {}
  virtual void postTick(const EngineState&) noexcept {}

  virtual void preRender(const EngineState&) noexcept {}
  virtual void render(const EngineState&) noexcept {}
  virtual void postRender(const EngineState&) noexcept {}

  virtual void onDebugUi(const EngineState&) noexcept {}

  template <typename T, typename... Args>
  T* addComponent(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Component, T>);
    return components_.add<T>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Scene, T>);
    return sceneStack_.add<T>(this, engine_, std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  T* addSystem(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<System, T>);
    return systems_.add<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  std::optional<T*> findScene() const noexcept {
    return FindScene<T>(this);
  }

  template <ComponentType T>
  inline void updateComponents(const EngineState& state) noexcept {
    components_.update<T>(state);
    std::for_each(
        sceneStack_.stack().begin(), sceneStack_.stack().end(),
        [&state](auto& scene) { scene->template updateComponents<T>(state); });
  }

  template <EngineStage S>
  inline void updateComponents(const EngineState& state) noexcept {
    components_.update<S>(state);
    std::for_each(
        sceneStack_.stack().begin(), sceneStack_.stack().end(),
        [&state](auto& scene) { scene->template updateComponents<S>(state); });
  }

  template <EngineStage S>
  inline void updateSystems(const EngineState& state) noexcept {
    systems_.update<S>(state);
    std::for_each(
        sceneStack_.stack().begin(), sceneStack_.stack().end(),
        [&state](auto& scene) { scene->template updateSystems<S>(state); });
  }

  template <SystemType T>
  inline void updateSystems(const EngineState& state) noexcept {
    systems_.update<T>(state);
    std::for_each(
        sceneStack_.stack().begin(), sceneStack_.stack().end(),
        [&state](auto& scene) { scene->template updateSystems<T>(state); });
  }

  const SceneStack& children() const noexcept { return sceneStack_; }

  SceneStack& children() noexcept { return sceneStack_; }

  const ComponentManager* components() const noexcept { return &components_; }

  Engine* engine() noexcept {
    return engine_ ? engine_ : parent_ ? parent_->engine_ : nullptr;
  }

  const Engine* engine() const noexcept {
    return engine_ ? engine_ : parent_ ? parent_->engine_ : nullptr;
  }

  bool isComplete() const noexcept { return flags_.isComplete(); }

  bool isError() const noexcept { return flags_.isError(); }

  bool isPaused() const noexcept { return flags_.isPaused(); }

  bool isRendering() const noexcept { return flags_.isRendering(); }

  bool isRunning() const noexcept { return flags_.isRunning(); }

  bool isTicking() const noexcept { return flags_.isTicking(); }

  Status layer(Layer) noexcept;

  Layer layer() const noexcept { return flags_.layer(); }

  const Scene* parent() const noexcept { return parent_; }

  void removeStaleScenes() noexcept {
    std::for_each(sceneStack_.stack().begin(), sceneStack_.stack().end(),
                  [](auto& scene) { scene->removeStaleScenes(); });
    constexpr auto predicate = [](const auto& scene) {
      return !(scene->isRunning() || scene->isPaused());
    };
    auto end =
        std::remove_if(sceneStack_.begin(), sceneStack_.end(), predicate);
    sceneStack_.stack().erase(end, sceneStack_.end());
  }

  time_t runtime() const noexcept override;

  Status state(State) noexcept;

  State state() const noexcept { return flags_.state(); }

  const SystemManager* systems() const noexcept { return &systems_; }

 private:
  ComponentManager components_;
  SystemManager systems_;
  SceneStack sceneStack_;

  Scene* parent_ = nullptr;
  Engine* engine_ = nullptr;

  time_t runtime_;
  Flags flags_;

  Status onSceneLayerChange(
      const SceneEvent<SceneEventType::LayerChange>&) noexcept;

  Status onSceneStateChange(
      const SceneEvent<SceneEventType::StateChange>&) noexcept;

  Status removeScene(Scene* scene) noexcept;
};

using SceneStack = Scene::SceneStack;

inline std::string to_string(Scene::Layer layer) noexcept {
  switch (layer) {
    case Scene::Layer::Simulation:
      return "Scene::Layer::Simulation";
    case Scene::Layer::UI:
      return "Scene::Layer::UI";
    case Scene::Layer::Debug:
      return "Scene::Layer::Debug";
    default:
      return "Unknown scene layer";
  }
}

inline std::string to_string(Scene::State state) noexcept {
  switch (state) {
    case Scene::State::Running:
      return "Scene::State::Running";
    case Scene::State::Pause:
      return "Scene::State::Pause";
    case Scene::State::Error:
      return "Scene::State::Error";
    case Scene::State::Complete:
      return "Scene::State::Complete";
    default:
      return "Unknown scene state";
  }
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_H_
