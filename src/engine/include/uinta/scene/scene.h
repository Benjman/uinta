#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_

#include <absl/log/log.h>
#include <absl/strings/str_format.h>
#include <absl/synchronization/mutex.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "uinta/flags.h"
#include "uinta/reflect.h"
#include "uinta/runtime_getter.h"
#include "uinta/scene/scene_events.h"
#include "uinta/scene/scene_layer.h"
#include "uinta/scene/scene_state.h"
#include "uinta/types.h"

namespace uinta {

class Engine;
class SceneSystem;

class Scene : public RuntimeGetter {
 public:
  class SceneStack {
    using Stack = std::vector<std::unique_ptr<Scene>>;

   public:
    template <typename T>
    static std::optional<T*> Find(const Scene* scene, bool recursive = true) noexcept {
      // If the scene being searched for exists in the scene tree, this will
      // eventually produce a valid result. Otherwise it will search the
      // children / leafs. If a valid value still is not returned, it searches
      // from the root.

      static_assert(std::is_base_of_v<Scene, T>);

      // Check if current scene is the one being searched for:
      if (auto* casted = dynamic_cast<const T*>(scene)) {
        return const_cast<T*>(casted);
      }

      // Iterate through child / leaf scenes, to check if any are the search
      // target:
      for (const auto& child : scene->children_.stack_) {
        if (auto result = Find<T>(child.get(), false)) {
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
        return Find<T>(scene->root(), false);
      }

      return std::nullopt;
    }

    template <typename T, typename... Args>
    T* add(Scene* parent, Args&&... args) noexcept {
      static_assert(std::is_base_of_v<Scene, T>);
      assert(parent && "Parent is required when adding a scene.");
      auto scene = std::make_unique<T>(parent, std::forward<Args>(args)...);
      absl::MutexLock lock(mtx_);
      auto itr = stack_.begin();
      for (; itr != stack_.end(); ++itr) {
        if (itr->get()->layer() > scene->layer()) {
          break;
        }
      }
      itr = stack_.insert(itr, std::move(scene));
      return reinterpret_cast<T*>(itr->get());
    }

    bool contains(const Scene* scene) noexcept {
      return std::ranges::find_if(stack_.begin(), stack_.end(), [&scene](const auto& s) { return scene == s.get(); }) !=
             stack_.end();
    }

    void remove(const Scene* scene) noexcept {
      if (scene == nullptr) {
        return;
      }
      absl::MutexLock lock(mtx_);
      std::erase_if(stack_, [&scene](const auto& s) { return s.get() == scene; });
    }

    Stack& stack() noexcept { return stack_; }

    [[nodiscard]] auto begin() const noexcept { return stack_.begin(); }

    [[nodiscard]] auto end() const noexcept { return stack_.end(); }

    auto begin() noexcept { return stack_.begin(); }

    auto end() noexcept { return stack_.end(); }

    [[nodiscard]] auto size() const noexcept { return stack_.size(); }

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

    [[nodiscard]] bool isComplete() const noexcept { return state() == SceneState::Complete; }

    [[nodiscard]] bool isPaused() const noexcept { return state() == SceneState::Pause; }

    [[nodiscard]] bool isRunning() const noexcept { return state() == SceneState::Running; }

    [[nodiscard]] bool isRendering() const noexcept { return isRunning(); }

    [[nodiscard]] bool isTicking() const noexcept { return isRunning(); }

    [[nodiscard]] SceneLayer layer() const noexcept {
      return static_cast<SceneLayer>((flags_ & Masks::SceneLayer) >> Shifts::SceneLayer);
    }

    SceneLayer layer(SceneLayer l) noexcept {
      flags_ &= ~Masks::SceneLayer;
      flags_ |= static_cast<value_type>(l) << Shifts::SceneLayer;
      return layer();
    }

    [[nodiscard]] SceneState state() const noexcept {
      return static_cast<SceneState>((flags_ & Masks::SceneState) >> Shifts::SceneState);
    }

    SceneState state(SceneState s) noexcept {
      flags_ &= ~Masks::SceneState;
      flags_ |= static_cast<value_type>(s) << Shifts::SceneState;
      return state();
    }

   private:
    std::atomic<value_type> flags_;
  };

  explicit Scene(Engine*, SceneLayer = SceneLayer::Debug, SceneState = SceneState::Running) noexcept;

  virtual ~Scene() noexcept;
  Scene(const Scene&) noexcept = delete;
  Scene& operator=(const Scene&) noexcept = delete;
  Scene(Scene&&) noexcept = delete;
  Scene& operator=(Scene&&) noexcept = delete;

  virtual void preTick(time_t /*unused*/) noexcept {}
  virtual void tick(time_t /*unused*/) noexcept {}
  virtual void postTick(time_t /*unused*/) noexcept {}

  virtual void preRender(time_t /*unused*/) noexcept {}
  virtual void render(time_t /*unused*/) noexcept {}
  virtual void postRender(time_t /*unused*/) noexcept {}

  virtual void onDebugUi() noexcept {}

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    auto* scene = children_.add<T>(this, std::forward<Args>(args)...);
    notifyAdded(scene);
    return scene;
  }

  template <typename T>
  std::optional<T*> findScene(bool recursive = true) const noexcept {
    return SceneStack::Find<T>(this, recursive);
  }

  template <typename T>
  T* findSceneOrThrow(bool recursive = true) const {
    if (std::optional<T*> opt = findScene<T>(recursive); opt.has_value()) {
      return opt.value();
    }
    throw std::runtime_error(absl::StrFormat("Required scene not found: '%s'", demangleTypeName(typeid(T).name())));
  }

  [[nodiscard]] const SceneStack& children() const noexcept { return children_; }

  SceneStack& children() noexcept { return children_; }

  Engine* engine() noexcept {
    if (engine_ != nullptr) {
      return engine_;
    }
    if (parent_ != nullptr) {
      return parent_->engine_;
    }
    return nullptr;
  }

  [[nodiscard]] const Engine* engine() const noexcept {
    if (engine_ != nullptr) {
      return engine_;
    }
    if (parent_ != nullptr) {
      return parent_->engine_;
    }
    return nullptr;
  }

  [[nodiscard]] bool isComplete() const noexcept { return flags_.isComplete(); }

  [[nodiscard]] bool isPaused() const noexcept { return flags_.isPaused(); }

  [[nodiscard]] bool isRendering() const noexcept { return flags_.isRendering(); }

  [[nodiscard]] bool isRunning() const noexcept { return flags_.isRunning(); }

  [[nodiscard]] bool isTicking() const noexcept { return flags_.isTicking(); }

  void layer(SceneLayer) noexcept;

  [[nodiscard]] SceneLayer layer() const noexcept { return flags_.layer(); }

  [[nodiscard]] virtual std::string name() const noexcept { return demangleTypeName(typeid(*this).name()); }

  [[nodiscard]] const Scene* parent() const noexcept { return parent_; }

  [[nodiscard]] const Scene* root() const noexcept { return (parent_ != nullptr) ? parent_->root() : this; }

  Scene* root() noexcept { return (parent_ != nullptr) ? parent_->root() : this; }

  [[nodiscard]] time_t runtime() const noexcept override;

  void state(SceneState) noexcept;

  [[nodiscard]] SceneState state() const noexcept { return flags_.state(); }

 protected:
  explicit Scene(Scene*, SceneLayer = SceneLayer::Debug, SceneState = SceneState::Running) noexcept;

 private:
  friend class SceneSystem;

  SceneStack children_;

  Scene* parent_ = nullptr;
  Engine* engine_ = nullptr;

  Flags flags_;

  SceneSystem* sceneSystem() noexcept;

  void notifyAdded(const Scene*) noexcept;
};

#define SCENE_NAME typeid(*this).name()

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_
