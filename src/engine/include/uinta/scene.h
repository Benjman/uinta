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
  using SceneFlag = u8;

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
                                  bool seekRoot = true) noexcept {
      static_assert(std::is_base_of<Scene, T>::value,
                    "T must be a derived class of Scene");

      if (auto* casted = dynamic_cast<const T*>(scene); casted) {
        return const_cast<T*>(casted);
      }

      const auto* root = scene;
      while (seekRoot && root->parent_) {
        if (auto* casted = dynamic_cast<const T*>(root); casted) {
          return const_cast<T*>(casted);
        }
        root = scene->parent_;
      }

      for (const auto& scene : root->sceneStack_.stack_) {
        if (auto* casted = dynamic_cast<T*>(scene.get()); casted) {
          return const_cast<T*>(casted);
        }
      }

      for (const auto& scene : root->sceneStack_.stack_) {
        if (auto result = Find<T>(scene.get(), false); result) {
          return result;
        }
      }

      return {};
    }

    template <typename T, typename... Args>
    T* add(Scene* parent, Args&&... args) noexcept {
      static_assert(std::is_base_of<Scene, T>::value,
                    "T must be a derived class of Scene");
      assert(parent && "`Scene* parent` is required.");
      auto scene = std::make_unique<T>(parent, std::forward<Args>(args)...);
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

  static constexpr std::array<Layer, 3> RenderOrder = {
      Layer::Simulation,
      Layer::UI,
      Layer::Debug,
  };

  template <typename T>
  static std::optional<T*> FindScene(const Scene* scene) noexcept {
    return SceneStack::Find<T>(scene);
  }

  explicit Scene(Scene* parent = nullptr, Layer = Layer::Debug) noexcept;

  explicit Scene(const Engine* engine, Layer = Layer::Debug) noexcept;

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
  T* addScene(Scene* parent, Args&&... args) noexcept {
    return sceneStack_.add<T>(parent ? parent : this,
                              std::forward<Args>(args)...);
  }

  SceneStack& children() noexcept { return sceneStack_; }

  const Scene* parent() const noexcept { return parent_; }

  template <typename T>
  std::optional<T*> findScene() const noexcept {
    return FindScene<T>(this);
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

  const Engine* engine() const noexcept {
    return engine_ ? engine_ : parent_ ? parent_->engine_ : nullptr;
  }

 private:
  SceneStack sceneStack_;
  SceneFlag flags_ = 0;

  Scene* parent_ = nullptr;
  const Engine* engine_ = nullptr;

  // The application runtime when the scene started running.
  const f32 startedOn_ = 0;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_H_
