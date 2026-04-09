#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_SYSTEM_H_

#include <deque>
#include <memory>
#include <utility>

#include "uinta/engine/engine_stage.h"
#include "uinta/engine/service_handle.h"
#include "uinta/scene/scene.h"
#include "uinta/types.h"

namespace uinta {

class Engine;

// Owns the root scenes. Only the front root is active; once it completes, the
// next root takes over.
class SceneSystem {
  using Roots = std::deque<std::unique_ptr<Scene>>;

 public:
  explicit SceneSystem(Engine*) noexcept;

  ~SceneSystem() noexcept;
  SceneSystem(const SceneSystem&) noexcept = delete;
  SceneSystem& operator=(const SceneSystem&) noexcept = delete;
  SceneSystem(SceneSystem&&) noexcept = delete;
  SceneSystem& operator=(SceneSystem&&) noexcept = delete;

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Scene, T>);
    auto& root = roots_.emplace_back(std::make_unique<T>(engine_, std::forward<Args>(args)...));
    return static_cast<T*>(root.get());
  }

  Scene* active() noexcept { return roots_.empty() ? nullptr : roots_.front().get(); }

  [[nodiscard]] const Scene* active() const noexcept { return roots_.empty() ? nullptr : roots_.front().get(); }

  template <EngineStage S>
  void advance(time_t delta) noexcept {
    auto* scene = active();
    if (scene == nullptr) {
      return;
    }
    if constexpr (S == EngineStage::PreTick) {
      visit<&Scene::preTick>(scene, delta);
    } else if constexpr (S == EngineStage::Tick) {
      visit<&Scene::tick>(scene, delta);
    } else if constexpr (S == EngineStage::PostTick) {
      visit<&Scene::postTick>(scene, delta);
    } else if constexpr (S == EngineStage::PreRender) {
      visit<&Scene::preRender>(scene, delta);
    } else if constexpr (S == EngineStage::Render) {
      render(scene, delta);
    } else if constexpr (S == EngineStage::PostRender) {
      visit<&Scene::postRender>(scene, delta);
    }
  }

  // Destroys every scene (and its subtree) which has been marked `Complete`.
  void flush() noexcept;

  [[nodiscard]] const Roots& roots() const noexcept { return roots_; }

 private:
  ServiceHandle<SceneSystem> serviceHandle_;
  Engine* engine_;
  Roots roots_;

  template <void (Scene::*Hook)(time_t)>
  static void visit(Scene* scene, time_t delta) noexcept {
    if (!scene->isRunning()) {
      return;
    }
    (scene->*Hook)(delta);
    for (auto& child : scene->children()) {
      visit<Hook>(child.get(), delta);
    }
  }

  void render(Scene*, time_t delta) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_SYSTEM_H_
