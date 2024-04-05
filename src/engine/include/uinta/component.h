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

class PreTickComponent : public Component {
 public:
  static constexpr EngineStage Stage_ = EngineStage::PreTick;
  PreTickComponent() noexcept = default;
};

template <>
struct Component::stage_to_type<EngineStage::PreTick> {
  using type = PreTickComponent;
};

class TickComponent : public Component {
 public:
  static constexpr EngineStage Stage_ = EngineStage::Tick;
  TickComponent() noexcept = default;
};

template <>
struct Component::stage_to_type<EngineStage::Tick> {
  using type = TickComponent;
};

class PostTickComponent : public Component {
 public:
  static constexpr EngineStage Stage_ = EngineStage::PostTick;
  PostTickComponent() noexcept = default;
};

template <>
struct Component::stage_to_type<EngineStage::PostTick> {
  using type = PostTickComponent;
};

class PreRenderComponent : public Component {
 public:
  static constexpr EngineStage Stage_ = EngineStage::PreRender;
  PreRenderComponent() noexcept = default;
};

template <>
struct Component::stage_to_type<EngineStage::PreRender> {
  using type = PreRenderComponent;
};

class RenderComponent : public Component {
 public:
  static constexpr EngineStage Stage_ = EngineStage::Render;
  RenderComponent() noexcept = default;
};

template <>
struct Component::stage_to_type<EngineStage::Render> {
  using type = RenderComponent;
};

class PostRenderComponent : public Component {
 public:
  static constexpr EngineStage Stage_ = EngineStage::PostRender;
  PostRenderComponent() noexcept = default;
};

template <>
struct Component::stage_to_type<EngineStage::PostRender> {
  using type = PostRenderComponent;
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
    } else if constexpr (T::Stage_ == EngineStage::PreTick) {
      return reinterprit_cast<T*>(
          preTickComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Stage_ == EngineStage::Tick) {
      return reinterprit_cast<T*>(
          tickComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Stage_ == EngineStage::PostTick) {
      return reinterprit_cast<T*>(
          postTickComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Stage_ == EngineStage::PreRender) {
      return reinterprit_cast<T*>(
          preRenderComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Stage_ == EngineStage::Render) {
      return reinterprit_cast<T*>(
          renderComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Stage_ == EngineStage::PostRender) {
      return reinterprit_cast<T*>(
          postRenderComponents_
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
    else if constexpr (T::Stage_ == EngineStage::PreTick)
      preTickComponents_.erase(
          std::remove_if(preTickComponents_.begin(), preTickComponents_.end(),
                         typeCheck),
          preTickComponents_.end());
    else if constexpr (T::Stage_ == EngineStage::Tick)
      tickComponents_.erase(std::remove_if(tickComponents_.begin(),
                                           tickComponents_.end(), typeCheck),
                            tickComponents_.end());
    else if constexpr (T::Stage_ == EngineStage::PostTick)
      postTickComponents_.erase(
          std::remove_if(postTickComponents_.begin(), postTickComponents_.end(),
                         typeCheck),
          postTickComponents_.end());
    else if constexpr (T::Stage_ == EngineStage::PreRender)
      preRenderComponents_.erase(
          std::remove_if(preRenderComponents_.begin(),
                         preRenderComponents_.end(), typeCheck),
          preRenderComponents_.end());
    else if constexpr (T::Stage_ == EngineStage::Render)
      renderComponents_.erase(
          std::remove_if(renderComponents_.begin(), renderComponents_.end(),
                         typeCheck),
          renderComponents_.end());
    else if constexpr (T::Stage_ == EngineStage::PostRender)
      postRenderComponents_.erase(
          std::remove_if(postRenderComponents_.begin(),
                         postRenderComponents_.end(), typeCheck),
          postRenderComponents_.end());
  }

  template <EngineStage S>
  void updateComponents(const EngineState& state) noexcept {
    if constexpr (S == EngineStage::NewFrame) {
      std::for_each(
          newFrameComponents_.begin(), newFrameComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == EngineStage::PreTick) {
      std::for_each(
          preTickComponents_.begin(), preTickComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == EngineStage::Tick) {
      std::for_each(
          tickComponents_.begin(), tickComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == EngineStage::PostTick) {
      std::for_each(
          postTickComponents_.begin(), postTickComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == EngineStage::PreRender) {
      std::for_each(
          preRenderComponents_.begin(), preRenderComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == EngineStage::Render) {
      std::for_each(
          renderComponents_.begin(), renderComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == EngineStage::PostRender) {
      std::for_each(
          postRenderComponents_.begin(), postRenderComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    }
  }

 private:
  template <typename T>
  using ComponentStorage = std::vector<std::unique_ptr<T>>;

  ComponentStorage<NewFrameComponent> newFrameComponents_;
  ComponentStorage<PreTickComponent> preTickComponents_;
  ComponentStorage<TickComponent> tickComponents_;
  ComponentStorage<PostTickComponent> postTickComponents_;
  ComponentStorage<PreRenderComponent> preRenderComponents_;
  ComponentStorage<RenderComponent> renderComponents_;
  ComponentStorage<PostRenderComponent> postRenderComponents_;

  void logInvalidEngineStage() const noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_
