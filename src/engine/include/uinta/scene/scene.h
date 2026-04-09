#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "uinta/reflect.h"
#include "uinta/scene/scene_state.h"
#include "uinta/types.h"

namespace uinta {

class Engine;

// A node in the scene tree. Each scene owns its children, and covers one area
// of responsibility (e.g. world simulation, UI, debug tooling).
//
// The `SceneSystem` advances the active root each frame: hooks run depth-first
// in insertion order, and a scene that isn't running skips its whole subtree.
// Rendering runs three passes over the tree:
// `render()` (Simulation), then `renderUi()` (UI), then `renderDebug()`
// (Debug). A scene overrides whichever passes it contributes to.
//
// Setting a scene's state to `Complete` removes it (and its subtree) at the
// start of the next frame.
class Scene {
 public:
  using Children = std::vector<std::unique_ptr<Scene>>;

  explicit Scene(Engine*) noexcept;

  virtual ~Scene() noexcept = default;
  Scene(const Scene&) noexcept = delete;
  Scene& operator=(const Scene&) noexcept = delete;
  Scene(Scene&&) noexcept = delete;
  Scene& operator=(Scene&&) noexcept = delete;

  virtual void preTick(time_t /*unused*/) noexcept {}
  virtual void tick(time_t /*unused*/) noexcept {}
  virtual void postTick(time_t /*unused*/) noexcept {}

  virtual void preRender(time_t /*unused*/) noexcept {}
  virtual void render(time_t /*unused*/) noexcept {}
  virtual void renderUi(time_t /*unused*/) noexcept {}
  virtual void renderDebug(time_t /*unused*/) noexcept {}
  virtual void postRender(time_t /*unused*/) noexcept {}

  virtual void onDebugUi() noexcept {}

  template <typename T, typename... Args>
  T* addScene(Args&&... args) noexcept {
    static_assert(std::is_base_of_v<Scene, T>);
    auto& child = children_.emplace_back(std::make_unique<T>(this, std::forward<Args>(args)...));
    return static_cast<T*>(child.get());
  }

  [[nodiscard]] const Children& children() const noexcept { return children_; }

  Children& children() noexcept { return children_; }

  [[nodiscard]] Engine* engine() const noexcept { return engine_; }

  [[nodiscard]] const Scene* parent() const noexcept { return parent_; }

  [[nodiscard]] SceneState state() const noexcept { return state_; }

  void state(SceneState state) noexcept { state_ = state; }

  [[nodiscard]] bool isRunning() const noexcept { return state_ == SceneState::Running; }

  [[nodiscard]] bool isComplete() const noexcept { return state_ == SceneState::Complete; }

  [[nodiscard]] virtual std::string name() const noexcept { return demangleTypeName(typeid(*this).name()); }

 protected:
  explicit Scene(Scene* parent) noexcept;

 private:
  Children children_;
  Scene* parent_ = nullptr;
  Engine* engine_ = nullptr;
  SceneState state_ = SceneState::Running;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_H_
