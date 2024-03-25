#ifndef SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_
#define SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "uinta/engine_stage.h"
#include "uinta/engine_state.h"

namespace uinta {

class Component {
 public:
  template <EngineStage S>
  struct stage_to_type;

  Component() noexcept = default;
  Component(const Component&) = delete;
  Component(Component&&) = delete;
  Component& operator=(const Component&) = delete;
  Component& operator=(Component&&) = delete;
  virtual ~Component() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

class NewFrameComponent : public Component {
 public:
  static constexpr EngineStage Stage_ = EngineStage::NewFrame;
  NewFrameComponent() noexcept = default;
};

template <>
struct Component::stage_to_type<EngineStage::NewFrame> {
  using type = NewFrameComponent;
};

class ComponentManager {
  template <EngineStage S>
  using stage_to_type_t = typename Component::stage_to_type<S>::type;

 public:
  template <typename T, typename... Args>
  T* addComponent(Args&&... args) noexcept {
    static_assert(std::is_base_of<stage_to_type_t<T::EngineStage_>, T>::value,
                  "Component does not match its stage type");
    if constexpr (T::EngineStage_ == EngineStage::NewFrame) {
      return reinterprit_cast<T*>(
          newFrameComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else {
      static_assert(false && "`addComponent` not implemented for stage.");
      return nullptr;
    }
  }

  template <typename T>
  void removeComponent() noexcept {
    static_assert(std::is_base_of<stage_to_type_t<T::EngineStage_>, T>::value,
                  "Component does not match its stage type");
    const auto typeCheck = [](const auto& component) {
      return dynamic_cast<T*>(component.get()) != nullptr;
    };
    if constexpr (T::EngineStage_ == EngineStage::NewFrame)
      newFrameComponents_.erase(
          std::remove_if(newFrameComponents_.begin(), newFrameComponents_.end(),
                         typeCheck),
          newFrameComponents_.end());
  }

  template <EngineStage S>
  void updateComponents(const EngineState& state) noexcept {
    if constexpr (S == EngineStage::NewFrame) {
      std::for_each(
          newFrameComponents_.begin(), newFrameComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    }
  }

 private:
  template <typename T>
  using ComponentStorage = std::vector<std::unique_ptr<T>>;

  ComponentStorage<NewFrameComponent> newFrameComponents_;

  void logInvalidEngineStage() const noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_
