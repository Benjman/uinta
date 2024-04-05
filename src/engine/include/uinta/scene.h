#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_H_

#include <algorithm>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "uinta/lib/absl/status.h"
#include "uinta/lib/absl/synchronization.h"
#include "uinta/system.h"
#include "uinta/types.h"

namespace uinta {

class Engine;
class EngineState;
struct ViewportSizeChange;

class Scene : public SystemManager {
  using SceneFlag = bitflag8;

  static constexpr SceneFlag LayerShift = 0;
  static constexpr SceneFlag LayerMask = 0x7 << LayerShift;

  static constexpr SceneFlag StateShift = 3;
  static constexpr SceneFlag StateMask = 0x7 << StateShift;

 public:
  class SceneStack {
    using Stack = std::vector<std::unique_ptr<Scene>>;

   public:
    template <typename T>
    static std::optional<T*> Find(const Scene* scene,
                                  bool searchChildren) noexcept {
      static_assert(std::is_base_of<Scene, T>::value,
                    "T must be a derived class of Scene");
      for (const auto& scene : scene->sceneStack_.stack_) {
        T* casted = dynamic_cast<T*>(scene.get());
        if (casted) return casted;
      }

      if (searchChildren) {
        for (const auto& scene : scene->sceneStack_.stack_) {
          if (auto result = Find<T>(scene.get(), true); result) {
            return result;
          }
        }
      }

      return {};
    }

    template <typename T, typename... Args>
    T* add(Scene* parent, Args&&... args) noexcept {
      static_assert(std::is_base_of<Scene, T>::value,
                    "T must be a derived class of Scene");
      auto scene = std::make_unique<T>(std::forward<Args>(args)...);
      scene->parent_ = parent;
      scene->engine_ = parent->engine_;
      MutexLock lock(&mtx_);
      auto itr = stack_.begin();
      for (; itr != stack_.end(); ++itr)
        if (itr->get()->layer() > scene->layer()) break;
      stack_.insert(itr, std::move(scene));
      return reinterpret_cast<T*>(stack_.back().get());
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

    void removeStaleScenes() noexcept {
      constexpr auto predicate = [](const auto& scene) {
        return !(scene->isRunning() || scene->isPause());
      };
      stack_.erase(std::remove_if(stack_.begin(), stack_.end(), predicate),
                   stack_.end());
    }

    Stack& stack() noexcept { return stack_; }

   private:
    Stack stack_;
    Mutex mtx_;
  };

  enum class Layer : SceneFlag {
    Simulation,
    UI,
    Debug,
  };

  enum class State : SceneFlag {
    Complete,
    Error,
    Pause,
    Running,
  };

  static constexpr std::array<Scene::Layer, 3> RenderOrder = {
      Scene::Layer::Simulation,
      Scene::Layer::UI,
      Scene::Layer::Debug,
  };

  template <typename T>
  static std::optional<T*> FindScene(const Scene* scene,
                                     bool searchChildren = true) noexcept {
    return SceneStack::Find<T>(scene, searchChildren);
  }

  explicit Scene(Layer layer) noexcept {
    flags_ |= static_cast<SceneFlag>(layer) << LayerShift;
    flags_ |= static_cast<SceneFlag>(State::Running) << StateShift;
  }

  virtual ~Scene() noexcept = default;

  Scene(const Scene&) noexcept = delete;
  Scene& operator=(const Scene&) noexcept = delete;
  Scene(Scene&&) noexcept = delete;
  Scene& operator=(Scene&&) noexcept = delete;

  virtual void preTick(const EngineState&) noexcept {}
  virtual void tick(const EngineState&) noexcept {}
  virtual void postTick(const EngineState&) noexcept {}

  virtual void preRender(const EngineState&) noexcept {}
  virtual void render(const EngineState&) noexcept {}
  virtual void postRender(const EngineState&) noexcept {}

  virtual void onViewportSizeChange(const ViewportSizeChange&) noexcept {}

  Layer layer() const noexcept {
    return static_cast<Layer>((flags_ & LayerMask) >> LayerShift);
  }

  State state() const noexcept {
    return static_cast<State>((flags_ & StateMask) >> StateShift);
  }

  Status state(State) noexcept;

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    return sceneStack_.add<T>(this, std::forward<Args>(args)...);
  }

  Engine* engine() const noexcept { return engine_; }

  void engine(Engine* engine) noexcept {
    engine_ = engine;
    std::for_each(sceneStack_.stack().begin(), sceneStack_.stack().end(),
                  [engine](auto& scene) { scene->engine(engine); });
  }

  SceneStack& children() noexcept { return sceneStack_; }

  const Scene* parent() const noexcept { return parent_; }

  template <typename T>
  std::optional<T*> findScene(bool searchChildren = true) const noexcept {
    return FindScene<T>(this, searchChildren);
  }

  void removeScene(const Scene* scene) noexcept {
    if (sceneStack_.contains(scene)) {
      sceneStack_.remove(scene);
    } else if (parent_) {
      parent_->removeScene(scene);
    }
  }

  void removeStaleScenes() noexcept { sceneStack_.removeStaleScenes(); }

  bool isComplete() const noexcept { return state() == State::Complete; }

  bool isError() const noexcept { return state() == State::Error; }

  bool isPause() const noexcept { return state() == State::Pause; }

  bool isRendering() const noexcept { return isRunning(); }

  bool isRunning() const noexcept { return state() == State::Running; }

  bool isTicking() const noexcept { return isRunning(); }

 private:
  SceneStack sceneStack_;
  SceneFlag flags_ = 0;

  Engine* engine_ = nullptr;
  Scene* parent_ = nullptr;

  Status validNewState(State) const noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_H_
