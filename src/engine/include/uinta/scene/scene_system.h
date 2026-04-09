#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_SYSTEM_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_SYSTEM_H_

#include <deque>
#include <memory>
#include <utility>
#include <vector>

#include "uinta/engine/engine_stage.h"
#include "uinta/scene/scene.h"
#include "uinta/scene/scene_events.h"
#include "uinta/types.h"

namespace uinta {

class Engine;

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
    auto* scene = roots_.emplace_back(std::make_unique<T>(engine_, std::forward<Args>(args)...)).get();
    dispatchers_.dispatch<SceneEvent::SceneAdded>(SceneAddedEvent(scene));
    return reinterpret_cast<T*>(scene);
  }

  template <SceneEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    dispatchers_.template addListener<E>(std::forward<Args>(args)...);
  }

  Scene* active() noexcept { return roots_.empty() ? nullptr : roots_.front().get(); }

  const Scene* active() const noexcept { return roots_.empty() ? nullptr : roots_.front().get(); }

  template <EngineStage S>
  void advance(time_t delta) noexcept {
    if (auto* scene = active(); scene != nullptr) {
      if constexpr (S == EngineStage::PreTick) {
        preTick(scene, delta);
      } else if constexpr (S == EngineStage::Tick) {
        tick(scene, delta);
      } else if constexpr (S == EngineStage::PostTick) {
        postTick(scene, delta);
      } else if constexpr (S == EngineStage::PreRender) {
        preRender(scene, delta);
      } else if constexpr (S == EngineStage::Render) {
        render(scene, delta);
      } else if constexpr (S == EngineStage::PostRender) {
        postRender(scene, delta);
      }
    }
  }

  SceneDispatchers* dispatchers() noexcept { return &dispatchers_; }

  const SceneDispatchers* dispatchers() const noexcept { return &dispatchers_; }

  // Removes scenes which have finished since the last flush.
  void flush() noexcept;

  [[nodiscard]] size_t pendingRemovals() const noexcept { return pendingRemoval_.size(); }

  const Roots& roots() const noexcept { return roots_; }

 private:
  friend class Scene;

  Engine* engine_;
  SceneDispatchers dispatchers_;
  std::vector<const Scene*> pendingRemoval_;
  Roots roots_;

  // Invoked by `~Scene()` so a destroyed scene is never left pending removal.
  void forget(const Scene*) noexcept;

  void remove(const Scene*) noexcept;

  void preTick(Scene*, time_t delta) noexcept;
  void tick(Scene*, time_t delta) noexcept;
  void postTick(Scene*, time_t delta) noexcept;
  void preRender(Scene*, time_t delta) noexcept;
  void render(Scene*, time_t delta) noexcept;
  void renderLayer(Scene*, SceneLayer, time_t delta) noexcept;
  void postRender(Scene*, time_t delta) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_SYSTEM_H_
