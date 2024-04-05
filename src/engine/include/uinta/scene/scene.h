#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/log/log.h"
#include "absl/strings/str_format.h"
#include "absl/synchronization/mutex.h"
#include "uinta/component.h"
#include "uinta/flags.h"
#include "uinta/reflect.h"
#include "uinta/runtime_getter.h"
#include "uinta/scene/scene_events.h"
#include "uinta/scene/scene_layer.h"
#include "uinta/scene/scene_state.h"
#include "uinta/status.h"
#include "uinta/system.h"
#include "uinta/types.h"

namespace uinta {

class Engine;

class Scene : public RuntimeGetter {
 public:
  class SceneStack {
    using Stack = std::vector<std::unique_ptr<Scene>>;

   public:
    template <typename T>
    static std::optional<T*> Find(
        const Scene* scene, std::optional<std::string> name = std::nullopt,
        bool recursive = true) noexcept {
      // If the scene being searched for exists in the scene tree, this will
      // eventually produce a valid result. Otherwise it will search the
      // children / leafs. If a valid value still is not returned, it searches
      // from the root.

      static_assert(std::is_base_of_v<Scene, T>);

      // Check if current scene is the one being searched for:
      if (auto* casted = dynamic_cast<const T*>(scene)) {
        if (!name.has_value() || scene->name() == name.value()) {
          return const_cast<T*>(casted);
        }
      }

      // Iterate through child / leaf scenes, to check if any are the search
      // target:
      for (const auto& child : scene->children_.stack_) {
        if (auto result = Find<T>(child.get(), name, false)) {
          return result;
        }
      }

      // If a result still is not found, optionally allow to search the tree
      // from the root:
      if (recursive) {  // TODO: Do we really need this condition check? At a
                        // later date, scan the project for instances where this
                        // conditional check doesn't result in the same as the
                        // majority of others. If any significant instances
                        // exist, keep it. Otherwise get rid of it.
        return Find<T>(scene->root(), name, false);
      }

      return std::nullopt;
    }

    template <typename T, typename... Args>
    T* add(Scene* parent, Args&&... args) noexcept {
      static_assert(std::is_base_of_v<Scene, T>);
      assert(parent && "Parent is required when adding a scene.");
      auto scene = std::make_unique<T>(parent, std::forward<Args>(args)...);
      if (!scene->status_.ok()) {
        LOG(WARNING) << absl::StrFormat("Failed to add scene %s: %s",
                                        scene->name(),
                                        scene->status_.message());
        return nullptr;
      }
      absl::MutexLock lock(&mtx_);
      auto itr = stack_.begin();
      for (; itr != stack_.end(); ++itr)
        if (itr->get()->layer() > scene->layer()) break;
      itr = stack_.insert(itr, std::move(scene));
      return reinterpret_cast<T*>(itr->get());
    }

    bool contains(const Scene* scene) noexcept {
      return std::find_if(stack_.begin(), stack_.end(),
                          [&scene](const auto& s) {
                            return scene == s.get();
                          }) != stack_.end();
    }

    void remove(const Scene* scene) noexcept {
      if (!scene) return;
      absl::MutexLock lock(&mtx_);
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
    absl::Mutex mtx_;
  };

  struct Flags final {
    using value_type = u8;

    struct Shifts final {
      static constexpr value_type SceneLayer = 0;
      static constexpr value_type SceneState = 3;
    };

    struct Masks final {
      static constexpr value_type SceneLayer = 0x7 << Shifts::SceneLayer;
      static constexpr value_type SceneState = 0x7 << Shifts::SceneState;
    };

    AtomicFlagsOperations(0);

    bool isComplete() const noexcept { return state() == SceneState::Complete; }

    bool isPaused() const noexcept { return state() == SceneState::Pause; }

    bool isRunning() const noexcept { return state() == SceneState::Running; }

    bool isRendering() const noexcept { return isRunning(); }

    bool isTicking() const noexcept { return isRunning(); }

    SceneLayer layer() const noexcept {
      return static_cast<SceneLayer>((flags_ & Masks::SceneLayer) >>
                                     Shifts::SceneLayer);
    }

    SceneLayer layer(SceneLayer l) noexcept {
      flags_ &= ~Masks::SceneLayer;
      flags_ |= static_cast<value_type>(l) << Shifts::SceneLayer;
      return layer();
    }

    SceneState state() const noexcept {
      return static_cast<SceneState>((flags_ & Masks::SceneState) >>
                                     Shifts::SceneState);
    }

    SceneState state(SceneState s) noexcept {
      flags_ &= ~Masks::SceneState;
      flags_ |= static_cast<value_type>(s) << Shifts::SceneState;
      return state();
    }

   private:
    std::atomic<value_type> flags_;
  };

  explicit Scene(Engine*, std::string, SceneLayer = SceneLayer::Debug) noexcept;

  virtual ~Scene() noexcept = default;
  Scene(const Scene&) noexcept = delete;
  Scene& operator=(const Scene&) noexcept = delete;
  Scene(Scene&&) noexcept = delete;
  Scene& operator=(Scene&&) noexcept = delete;

  virtual void preTick(time_t) noexcept {}
  virtual void tick(time_t) noexcept {}
  virtual void postTick(time_t) noexcept {}

  virtual void preRender(time_t) noexcept {}
  virtual void render(time_t) noexcept {}
  virtual void postRender(time_t) noexcept {}

  virtual void onDebugUi() noexcept {}

  template <typename T, typename... Args>
  T* addComponent(Args&&... args) noexcept {
    return components_.add<T>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    auto itr = children_.add<T>(this, std::forward<Args>(args)...);
    dispatchers_.dispatch<SceneEvent::SceneAdded>(SceneAddedEvent(itr));
    return reinterpret_cast<T*>(itr);
  }

  template <typename T, typename... Args>
  T* addSystem(Args&&... args) noexcept {
    return systems_.add<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  std::optional<T*> findScene(std::optional<std::string> name = std::nullopt,
                              bool recursive = true) const noexcept {
    return SceneStack::Find<T>(this, name, recursive);
  }

  template <typename T>
  T* findSceneOrThrow(std::optional<std::string> name = std::nullopt,
                      bool recursive = true) const {
    if (std::optional<T*> opt = findScene<T>(name, recursive);
        opt.has_value()) {
      return opt.value();
    } else {
      throw std::runtime_error(
          absl::StrFormat("Required system not found: '%s'",
                          demangleTypeName(typeid(T).name())));
    }
  }

  template <ComponentType T>
  inline void updateComponents(time_t delta) noexcept {
    components_.update<T>(delta);
    std::for_each(
        children_.stack().begin(), children_.stack().end(),
        [delta](auto& scene) { scene->template updateComponents<T>(delta); });
  }

  template <EngineStage S>
  inline void updateComponents(time_t delta) noexcept {
    components_.update<S>(delta);
    std::for_each(
        children_.stack().begin(), children_.stack().end(),
        [delta](auto& scene) { scene->template updateComponents<S>(delta); });
  }

  template <EngineStage S>
  inline void updateSystems(time_t delta) noexcept {
    systems_.update<S>(delta);
    std::for_each(
        children_.stack().begin(), children_.stack().end(),
        [delta](auto& scene) { scene->template updateSystems<S>(delta); });
  }

  template <SystemType T>
  inline void updateSystems(time_t delta) noexcept {
    systems_.update<T>(delta);
    std::for_each(
        children_.stack().begin(), children_.stack().end(),
        [delta](auto& scene) { scene->template updateSystems<T>(delta); });
  }

  template <typename T>
  std::optional<T*> findComponent() const noexcept {
    if (auto result = components_.find<T>(); result.has_value()) {
      return result;
    }

    for (const auto& leaf : children_) {
      if (auto result = leaf->findComponent<T>(); result.has_value()) {
        return result;
      }
    }

    if (auto* root = this->root()) {
      return root->findComponent<T>();
    } else {
      return std::nullopt;
    }
  }

  template <typename T>
  std::optional<T*> findSystem() const noexcept {
    if (auto result = systems_.find<T>(); result.has_value()) {
      return result;
    }

    for (const auto& leaf : children_) {
      if (auto result = leaf->findSystem<T>(); result.has_value()) {
        return result;
      }
    }

    if (auto* root = this->root(); root != this) {
      return root->findSystem<T>();
    } else {
      return std::nullopt;
    }
  }

  template <typename T>
  T* findSystemOrThrow() const {
    if (auto system = findSystem<T>().value_or(nullptr); system) {
      return system;
    } else {
      throw std::runtime_error(
          absl::StrFormat("Required system not found: '%s'",
                          demangleTypeName(typeid(T).name())));
    }
  }

  const SceneStack& children() const noexcept { return children_; }

  SceneStack& children() noexcept { return children_; }

  const auto* components() const noexcept { return &components_; }

  auto* components() noexcept { return &components_; }

  SceneDispatchers* dispatchers() noexcept { return &dispatchers_; }

  const SceneDispatchers* dispatchers() const noexcept { return &dispatchers_; }

  Engine* engine() noexcept {
    return engine_ ? engine_ : parent_ ? parent_->engine_ : nullptr;
  }

  const Engine* engine() const noexcept {
    return engine_ ? engine_ : parent_ ? parent_->engine_ : nullptr;
  }

  bool isComplete() const noexcept { return flags_.isComplete(); }

  bool isPaused() const noexcept { return flags_.isPaused(); }

  bool isRendering() const noexcept { return flags_.isRendering(); }

  bool isRunning() const noexcept { return flags_.isRunning(); }

  bool isTicking() const noexcept { return flags_.isTicking(); }

  void layer(SceneLayer) noexcept;

  SceneLayer layer() const noexcept { return flags_.layer(); }

  std::string name() const noexcept { return name_; }

  const Scene* parent() const noexcept { return parent_; }

  void removeStaleScenes() noexcept {
    std::for_each(children_.stack().begin(), children_.stack().end(),
                  [](auto& scene) { scene->removeStaleScenes(); });
    constexpr auto predicate = [](const auto& scene) {
      return !(scene->isRunning() || scene->isPaused());
    };
    auto end = std::remove_if(children_.begin(), children_.end(), predicate);
    children_.stack().erase(end, children_.end());
  }

  const Scene* root() const noexcept {
    return parent_ ? parent_->root() : this;
  }

  Scene* root() noexcept { return parent_ ? parent_->root() : this; }

  time_t runtime() const noexcept override;

  void state(SceneState) noexcept;

  Status status() const noexcept { return status_; }

  SceneState state() const noexcept { return flags_.state(); }

  void setError(Status status) noexcept {
    assert(!status.ok());
    if (status_.ok()) {
      status_ = status;
    }
  }

  const auto* systems() const noexcept { return &systems_; }

  auto* systems() noexcept { return &systems_; }

 protected:
  explicit Scene(Scene*, std::string, SceneLayer = SceneLayer::Debug) noexcept;

 private:
  const std::string name_;
  ComponentManager components_;
  SystemManager systems_;
  SceneStack children_;
  SceneDispatchers dispatchers_;
  Status status_;

  Scene* parent_ = nullptr;
  Engine* engine_ = nullptr;

  Flags flags_;

  Status onSceneLayerChange(const SceneLayerChangeEvent&) noexcept;

  Status onSceneStateChange(const SceneStateChangeEvent&) noexcept;

  Status removeScene(Scene* scene) noexcept;
};

#define SCENE_NAME typeid(*this).name()

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_
