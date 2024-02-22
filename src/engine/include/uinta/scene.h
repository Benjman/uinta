#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_H_

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "uinta/component.h"
#include "uinta/lib/absl/status.h"
#include "uinta/lib/absl/synchronization.h"
#include "uinta/types.h"

namespace uinta {

class Engine;
class EngineState;
class Input;
struct ViewportSizeChange;

class Scene {
  using SceneFlag = bitflag8;
  using SceneStack = std::vector<std::unique_ptr<Scene>>;

  static constexpr SceneFlag LayerShift = 0;
  static constexpr SceneFlag LayerMask = 0x7 << LayerShift;

  static constexpr SceneFlag StateShift = 3;
  static constexpr SceneFlag StateMask = 0x7 << StateShift;

 public:
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
      Scene::Layer::Simulation, Scene::Layer::UI, Scene::Layer::Debug};

  template <typename T>
  static std::optional<T*> FindScene(const Scene* scene,
                                     bool searchChildren = true) noexcept {
    static_assert(std::is_base_of<Scene, T>::value,
                  "T must be a derived class of Scene");
    for (const auto& scene : scene->scenes_) {
      T* casted = dynamic_cast<T*>(scene.get());
      if (casted) return casted;
    }

    if (searchChildren) {
      for (const auto& scene : scene->scenes_) {
        if (auto result = FindScene<T>(scene.get()); result) {
          return result;
        }
      }
    }

    return {};
  }

  explicit Scene(Layer) noexcept;

  virtual ~Scene() noexcept = default;

  Scene(const Scene&) noexcept = delete;
  Scene& operator=(const Scene&) noexcept = delete;

  Scene(Scene&&) noexcept;
  Scene& operator=(Scene&&) noexcept;

  virtual void preTick(const EngineState&, const Input&) noexcept {}
  virtual void tick(const EngineState&, const Input&) noexcept {}
  virtual void postTick(const EngineState&, const Input&) noexcept {}

  virtual void preRender(const EngineState&, const Input&) noexcept {}
  virtual void render(const EngineState&, const Input&) noexcept {}
  virtual void postRender(const EngineState&, const Input&) noexcept {}

  virtual void onViewportSizeChange(const ViewportSizeChange&) noexcept {}

  Layer layer() const noexcept {
    return static_cast<Layer>((flags_ & LayerMask) >> LayerShift);
  }

  State state() const noexcept {
    return static_cast<State>((flags_ & StateMask) >> StateShift);
  }

  Status state(State) noexcept;

  template <typename T, typename... Args>
  void addScene(Args&&... args) {
    static_assert(std::is_base_of<Scene, T>::value,
                  "T must be a derived class of Scene");
    auto scene = std::make_unique<T>(std::forward<Args>(args)...);
    MutexLock lock(&mtx_);
    auto itr = scenes_.begin();
    for (; itr != scenes_.end(); ++itr)
      if (itr->get()->layer() > scene->layer()) break;
    scenes_.insert(itr, std::move(scene));
    scenes_.back()->engine_ = engine_;
    scenes_.back()->parent_ = this;
  }

  Engine* engine() const noexcept { return engine_; }

  SceneStack& scenes() noexcept { return scenes_; }

  const Scene* parent() const noexcept { return parent_; }

  template <typename T>
  std::optional<T*> findScene(bool searchChildren = true) const noexcept {
    return FindScene<T>(this, searchChildren);
  }

  ComponentList& components() noexcept { return components_; }

 private:
  ComponentList components_;
  SceneStack scenes_;
  Mutex mtx_;
  SceneFlag flags_ = 0;

  Engine* engine_ = nullptr;
  Scene* parent_ = nullptr;

  Status validNewState(State) const noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_H_
