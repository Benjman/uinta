#ifndef SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_
#define SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_

#include <algorithm>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

#include "absl/strings/str_format.h"
#include "uinta/engine/engine_stage.h"
#include "uinta/engine/engine_state.h"
#include "uinta/lib/absl/synchronization.h"
#include "uinta/runtime_getter.h"

namespace uinta {

class Component {
 public:
  enum class Type : u8 {
    Interval,
    NewFrame,
    PostRender,
    PostTick,
    PreRender,
    PreTick,
    Render,
    Tick,
  };

  template <Type S>
  struct component_type_to_class;

  virtual ~Component() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

using ComponentType = Component::Type;

class NewFrameComponent : public Component {
 public:
  static constexpr Type Type_ = Type::NewFrame;

  NewFrameComponent() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::NewFrame> {
  using type = NewFrameComponent;
};

class PreTickComponent : public Component {
 public:
  static constexpr Type Type_ = Type::PreTick;

  PreTickComponent() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::PreTick> {
  using type = PreTickComponent;
};

class TickComponent : public Component {
 public:
  static constexpr Type Type_ = Type::Tick;

  TickComponent() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::Tick> {
  using type = TickComponent;
};

class PostTickComponent : public Component {
 public:
  static constexpr Type Type_ = Type::PostTick;

  PostTickComponent() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::PostTick> {
  using type = PostTickComponent;
};

class PreRenderComponent : public Component {
 public:
  static constexpr Type Type_ = Type::PreRender;

  PreRenderComponent() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::PreRender> {
  using type = PreRenderComponent;
};

class RenderComponent : public Component {
 public:
  static constexpr Type Type_ = Type::Render;

  RenderComponent() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::Render> {
  using type = RenderComponent;
};

class PostRenderComponent : public Component {
 public:
  static constexpr Type Type_ = Type::PostRender;

  PostRenderComponent() noexcept = default;

  virtual void update(const EngineState&) noexcept = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::PostRender> {
  using type = PostRenderComponent;
};

class IntervalComponent : public Component {
 public:
  static constexpr Type Type_ = Type::Interval;

  explicit IntervalComponent(time_t interval, bool isOneShot = false) noexcept
      : interval_(interval) {
    assert(interval > 0);
    flags_.isOneShot(isOneShot);
  }

  virtual void update(const EngineState&) noexcept = 0;

  time_t interval() const noexcept { return interval_; }

  time_t nextExecutionTime() const noexcept { return nextExecutionTime_; }

  bool isComplete() const noexcept { return flags_.isTerminate(); }

  void isComplete(bool v) noexcept { flags_.isTerminate(v); }

 private:
  friend class ComponentManager;

  struct Flags final {
    using value_type = u8;

    bool isOneShot() const noexcept { return flags_ & OneShotMask; }
    void isOneShot(bool v) noexcept {
      flags_ &= ~OneShotMask;
      if (v) flags_ |= OneShotMask;
    }

    bool isTerminate() const noexcept { return flags_ & TerminateMask; }
    void isTerminate(bool v) noexcept {
      flags_ &= ~TerminateMask;
      if (v) flags_ |= TerminateMask;
    }

   private:
    static constexpr value_type OneShotMask = 1 << 0;
    static constexpr value_type TerminateMask = 1 << 1;

    std::atomic<value_type> flags_;
  } flags_;

  time_t interval_;
  time_t nextExecutionTime_ = 0;
};

template <>
struct Component::component_type_to_class<Component::Type::Interval> {
  using type = IntervalComponent;
};

class ComponentManager {
 public:
  explicit ComponentManager(const RuntimeGetter* runtimeGetter) noexcept
      : runtimeGetter_(runtimeGetter) {
    assert(runtimeGetter != nullptr && "`RuntimeGetter*` cannot be null.");
  }

  template <typename T>
  using ComponentStorage = std::vector<std::unique_ptr<T>>;

  struct CompareNextExecutionTime {
    bool operator()(
        const std::unique_ptr<IntervalComponent>& a,
        const std::unique_ptr<IntervalComponent>& b) const noexcept {
      return a->nextExecutionTime() > b->nextExecutionTime();
    }
  };

  using IntervalComponentQueue =
      std::priority_queue<std::unique_ptr<IntervalComponent>,
                          std::vector<std::unique_ptr<IntervalComponent>>,
                          CompareNextExecutionTime>;

  template <Component::Type S>
  using component_type_to_class_t =
      typename Component::component_type_to_class<S>::type;

  template <typename T, typename... Args>
  T* add(Args&&... args) noexcept {
    static_assert(
        std::is_base_of<component_type_to_class_t<T::Type_>, T>::value,
        "Component does not match its type");
    if constexpr (T::Type_ == Component::Type::NewFrame) {
      MutexLock lock(&newFrameComponentsMtx_);
      return reinterpret_cast<T*>(
          newFrameComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Type_ == Component::Type::PreTick) {
      MutexLock lock(&preTickComponentsMtx_);
      return reinterpret_cast<T*>(
          preTickComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Type_ == Component::Type::Tick) {
      MutexLock lock(&tickComponentsMtx_);
      return reinterpret_cast<T*>(
          tickComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Type_ == Component::Type::PostTick) {
      MutexLock lock(&postTickComponentsMtx_);
      return reinterpret_cast<T*>(
          postTickComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Type_ == Component::Type::PreRender) {
      MutexLock lock(&preRenderComponentsMtx_);
      return reinterpret_cast<T*>(
          preRenderComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Type_ == Component::Type::Render) {
      MutexLock lock(&renderComponentsMtx_);
      return reinterpret_cast<T*>(
          renderComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Type_ == Component::Type::PostRender) {
      MutexLock lock(&postRenderComponentsMtx_);
      return reinterpret_cast<T*>(
          postRenderComponents_
              .emplace_back(std::make_unique<T>(std::forward<Args>(args)...))
              .get());
    } else if constexpr (T::Type_ == Component::Type::Interval) {
      auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
      T* result = reinterpret_cast<T*>(ptr.get());
      ptr->nextExecutionTime_ = runtimeGetter_->runtime() + ptr->interval_;
      MutexLock lock(&intervalComponentQueueMtx_);
      intervalComponentQueue_.push(std::move(ptr));
      return result;
    } else {
      static_assert(false &&
                    "`ComponentManager::add()` not implemented for type.");
      return nullptr;
    }
  }

  template <typename T>
  void remove(T*& component) noexcept {  // NOLINT
    static_assert(
        std::is_base_of<component_type_to_class_t<T::Type_>, T>::value,
        "Component does not match its type");
    if constexpr (T::Type_ == Component::Type::NewFrame) {
      MutexLock lock(&newFrameComponentsMtx_);
      if (auto itr = std::find_if(
              newFrameComponents_.begin(), newFrameComponents_.end(),
              [component](const std::unique_ptr<NewFrameComponent>& ptr) {
                return ptr.get() == component;
              });
          itr != newFrameComponents_.end()) {
        newFrameComponents_.erase(itr);
      }
    } else if constexpr (T::Type_ == Component::Type::PreTick) {
      MutexLock lock(&preTickComponentsMtx_);
      if (auto itr = std::find_if(
              preTickComponents_.begin(), preTickComponents_.end(),
              [component](const std::unique_ptr<PreTickComponent>& ptr) {
                return ptr.get() == component;
              });
          itr != preTickComponents_.end()) {
        preTickComponents_.erase(itr);
      }
    } else if constexpr (T::Type_ == Component::Type::Tick) {
      MutexLock lock(&tickComponentsMtx_);
      if (auto itr = std::find_if(
              tickComponents_.begin(), tickComponents_.end(),
              [component](const std::unique_ptr<TickComponent>& ptr) {
                return ptr.get() == component;
              });
          itr != tickComponents_.end()) {
        tickComponents_.erase(itr);
      }
    } else if constexpr (T::Type_ == Component::Type::PostTick) {
      MutexLock lock(&postTickComponentsMtx_);
      if (auto itr = std::find_if(
              postTickComponents_.begin(), postTickComponents_.end(),
              [component](const std::unique_ptr<PostTickComponent>& ptr) {
                return ptr.get() == component;
              });
          itr != postTickComponents_.end()) {
        postTickComponents_.erase(itr);
      }
    } else if constexpr (T::Type_ == Component::Type::PreRender) {
      MutexLock lock(&preRenderComponentsMtx_);
      if (auto itr = std::find_if(
              preRenderComponents_.begin(), preRenderComponents_.end(),
              [component](const std::unique_ptr<PreRenderComponent>& ptr) {
                return ptr.get() == component;
              });
          itr != preRenderComponents_.end()) {
        preRenderComponents_.erase(itr);
      }
    } else if constexpr (T::Type_ == Component::Type::Render) {
      MutexLock lock(&renderComponentsMtx_);
      if (auto itr = std::find_if(
              renderComponents_.begin(), renderComponents_.end(),
              [component](const std::unique_ptr<RenderComponent>& ptr) {
                return ptr.get() == component;
              });
          itr != renderComponents_.end()) {
        renderComponents_.erase(itr);
      }
    } else if constexpr (T::Type_ == Component::Type::PostRender) {
      MutexLock lock(&postRenderComponentsMtx_);
      if (auto itr = std::find_if(
              postRenderComponents_.begin(), postRenderComponents_.end(),
              [component](const std::unique_ptr<PostRenderComponent>& ptr) {
                return ptr.get() == component;
              });
          itr != postRenderComponents_.end()) {
        postRenderComponents_.erase(itr);
      }
    } else if constexpr (T::Type_ == Component::Type::Interval) {
      MutexLock lock(&intervalComponentQueueMtx_);
      IntervalComponentQueue newQueue;
      while (!intervalComponentQueue_.empty()) {
        auto top = std::move(const_cast<std::unique_ptr<IntervalComponent>&>(
            intervalComponentQueue_.top()));
        intervalComponentQueue_.pop();
        if (top.get() != component) {
          newQueue.push(std::move(top));
        }
      }
      std::swap(intervalComponentQueue_, newQueue);
    } else {
      static_assert(false &&
                    "`ComponentManager::remove()` not implemented for type.");
    }
    component = nullptr;
  }

  template <typename T>
  std::optional<T*> find() const noexcept {
    static_assert(
        std::is_base_of<component_type_to_class_t<T::Type_>, T>::value,
        "Component does not match its type");
    if constexpr (T::Type_ == Component::Type::NewFrame) {
      if (auto itr = std::find_if(
              newFrameComponents_.begin(), newFrameComponents_.end(),
              [](const auto& component) {
                return dynamic_cast<const T*>(component.get());
              });
          itr != newFrameComponents_.end())
        return reinterpret_cast<T*>(itr->get());
    } else if constexpr (T::Type_ == Component::Type::PostRender) {
      if (auto itr = std::find_if(
              postRenderComponents_.begin(), postRenderComponents_.end(),
              [](const auto& component) {
                return dynamic_cast<const T*>(component.get());
              });
          itr != postRenderComponents_.end())
        return reinterpret_cast<T*>(itr->get());
    } else if constexpr (T::Type_ == Component::Type::PostTick) {
      if (auto itr = std::find_if(
              postTickComponents_.begin(), postTickComponents_.end(),
              [](const auto& component) {
                return dynamic_cast<const T*>(component.get());
              });
          itr != postTickComponents_.end())
        return reinterpret_cast<T*>(itr->get());
    } else if constexpr (T::Type_ == Component::Type::PreRender) {
      if (auto itr = std::find_if(
              preRenderComponents_.begin(), preRenderComponents_.end(),
              [](const auto& component) {
                return dynamic_cast<const T*>(component.get());
              });
          itr != preRenderComponents_.end())
        return reinterpret_cast<T*>(itr->get());
    } else if constexpr (T::Type_ == Component::Type::PreTick) {
      if (auto itr =
              std::find_if(preTickComponents_.begin(), preTickComponents_.end(),
                           [](const auto& component) {
                             return dynamic_cast<const T*>(component.get());
                           });
          itr != preTickComponents_.end())
        return reinterpret_cast<T*>(itr->get());
    } else if constexpr (T::Type_ == Component::Type::Render) {
      if (auto itr =
              std::find_if(renderComponents_.begin(), renderComponents_.end(),
                           [](const auto& component) {
                             return dynamic_cast<const T*>(component.get());
                           });
          itr != renderComponents_.end())
        return reinterpret_cast<T*>(itr->get());
    } else if constexpr (T::Type_ == Component::Type::Tick) {
      if (auto itr =
              std::find_if(tickComponents_.begin(), tickComponents_.end(),
                           [](const auto& component) {
                             return dynamic_cast<const T*>(component.get());
                           });
          itr != tickComponents_.end())
        return reinterpret_cast<T*>(itr->get());
    } else {
      static_assert(false &&
                    "`ComponentManager::find()` not implemented for type.");
    }
    return std::nullopt;
  }

  template <EngineStage S>
  void update(const EngineState& state) noexcept {
    constexpr std::array<Component::Type, 7> EngineStageToType = {
        Component::Type::PreTick,  Component::Type::Tick,
        Component::Type::PostTick, Component::Type::PreRender,
        Component::Type::Render,   Component::Type::PostRender,
    };

    update<EngineStageToType[static_cast<u8>(S)]>(state);
  }

  template <Component::Type S>
  void update(const EngineState& state) noexcept {
    updateIntervalComponents(state);

    if constexpr (S == Component::Type::NewFrame) {
      std::for_each(
          newFrameComponents_.begin(), newFrameComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == Component::Type::PreTick) {
      std::for_each(
          preTickComponents_.begin(), preTickComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == Component::Type::Tick) {
      std::for_each(
          tickComponents_.begin(), tickComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == Component::Type::PostTick) {
      std::for_each(
          postTickComponents_.begin(), postTickComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == Component::Type::PreRender) {
      std::for_each(
          preRenderComponents_.begin(), preRenderComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == Component::Type::Render) {
      std::for_each(
          renderComponents_.begin(), renderComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    } else if (S == Component::Type::PostRender) {
      std::for_each(
          postRenderComponents_.begin(), postRenderComponents_.end(),
          [&state](auto& component) noexcept { component->update(state); });
    }
  }

  std::size_t allSize() const noexcept {
    // clang-format off
    return newFrameComponents_.size() +
      preTickComponents_.size() +
      tickComponents_.size() +
      postTickComponents_.size() +
      preRenderComponents_.size() +
      renderComponents_.size() +
      postRenderComponents_.size() +
      intervalComponentQueue_.size();
    // clang-format on
  }

  const ComponentStorage<NewFrameComponent>& newFrameComponents()
      const noexcept {
    return newFrameComponents_;
  }

  const ComponentStorage<PreTickComponent>& preTickComponents() const noexcept {
    return preTickComponents_;
  }

  const ComponentStorage<TickComponent>& tickComponents() const noexcept {
    return tickComponents_;
  }

  const ComponentStorage<PostTickComponent>& postTickComponents()
      const noexcept {
    return postTickComponents_;
  }

  const ComponentStorage<PreRenderComponent>& preRenderComponents()
      const noexcept {
    return preRenderComponents_;
  }

  const ComponentStorage<RenderComponent>& renderComponents() const noexcept {
    return renderComponents_;
  }

  const ComponentStorage<PostRenderComponent>& postRenderComponents()
      const noexcept {
    return postRenderComponents_;
  }

  const IntervalComponentQueue& intervalComponentQueue() const noexcept {
    return intervalComponentQueue_;
  }

 private:
  Mutex newFrameComponentsMtx_;
  ComponentStorage<NewFrameComponent> newFrameComponents_;
  Mutex preTickComponentsMtx_;
  ComponentStorage<PreTickComponent> preTickComponents_;
  Mutex tickComponentsMtx_;
  ComponentStorage<TickComponent> tickComponents_;
  Mutex postTickComponentsMtx_;
  ComponentStorage<PostTickComponent> postTickComponents_;
  Mutex preRenderComponentsMtx_;
  ComponentStorage<PreRenderComponent> preRenderComponents_;
  Mutex renderComponentsMtx_;
  ComponentStorage<RenderComponent> renderComponents_;
  Mutex postRenderComponentsMtx_;
  ComponentStorage<PostRenderComponent> postRenderComponents_;
  Mutex intervalComponentQueueMtx_;
  IntervalComponentQueue intervalComponentQueue_;

  const RuntimeGetter* runtimeGetter_;

  void updateIntervalComponents(const EngineState& state) noexcept {
    if (intervalComponentQueue_.empty()) return;
    auto runtime = state.runtime();
    while (!intervalComponentQueue_.empty() &&
           runtime >= intervalComponentQueue_.top()->nextExecutionTime()) {
      auto component =
          std::move(const_cast<std::unique_ptr<IntervalComponent>&>(
              intervalComponentQueue_.top()));

      intervalComponentQueue_.pop();

      component->update(state);
      if (!component->isComplete()) {
        count_t missedIntervalCount = 0;
        while (component->nextExecutionTime_ += component->interval_ < runtime)
          missedIntervalCount++;
        if (missedIntervalCount)
          logWarning(
              absl::StrFormat("Interval component missed %u interval calls.",
                              missedIntervalCount)
                  .c_str());
        MutexLock lock(&intervalComponentQueueMtx_);
        intervalComponentQueue_.push(std::move(component));
      }
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_COMPONENT_H_
