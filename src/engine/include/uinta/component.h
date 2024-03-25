#ifndef SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_
#define SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

namespace uinta {

class EngineState;

class Component {
 public:
  enum class Stage {
    PreTick,
    Tick,
    PostTick,
    NewFrame,
    PreRender,
    Render,
    PostRender,
  };

  explicit Component(Stage stage) noexcept : stage_(stage) {}

  virtual ~Component() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;

  Stage stage() const noexcept { return stage_; }

 private:
  const Stage stage_;
};

class PreTickComponent : public Component {
 public:
  PreTickComponent() noexcept : Component(Stage::PreTick) {}
};

class TickComponent : public Component {
 public:
  TickComponent() noexcept : Component(Stage::Tick) {}
};

class PostTickComponent : public Component {
 public:
  PostTickComponent() noexcept : Component(Stage::PostTick) {}
};

class NewFrameComponent : public Component {
 public:
  NewFrameComponent() noexcept : Component(Stage::NewFrame) {}
};

class PreRenderComponent : public Component {
 public:
  PreRenderComponent() noexcept : Component(Stage::PreRender) {}
};

class RenderComponent : public Component {
 public:
  RenderComponent() noexcept : Component(Stage::Render) {}
};

class PostRenderComponent : public Component {
 public:
  PostRenderComponent() noexcept : Component(Stage::PostRender) {}
};

class ComponentList {
 public:
  template <typename T, typename... Args>
  void add(Args&&... args) noexcept {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must be a derived class of Component");
    components_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
  }

  template <typename T>
  void remove() noexcept {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must be a derived class of Component");
    components_.erase(
        std::remove_if(components_.begin(), components_.end(),
                       [](const auto& component) {
                         return dynamic_cast<T*>(component.get()) != nullptr;
                       }),
        components_.end());
  }

  void update(Component::Stage stage, const EngineState& state) noexcept {
    std::for_each(components_.begin(), components_.end(),
                  [&stage, &state](auto& component) {
                    if (component->stage() == stage) {
                      component->update(state);
                    }
                  });
  }

 private:
  std::vector<std::unique_ptr<Component>> components_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_
