#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_

#include <eventpp/eventdispatcher.h>

#include <typeindex>
#include <utility>

#include "uinta/scene/scene_layer.h"
#include "uinta/types.h"

namespace uinta {

class EngineState;
class Scene;

struct RenderComplete {
  const EngineState* state;
  time_t runtime;
};

struct RenderLayerChange {
  SceneLayer layer;
};

struct TickComplete {
  const EngineState* state;
  time_t runtime;
};

struct ViewportSizeChange {
  u32 width;
  u32 height;

  [[nodiscard]] f32 aspect() const noexcept {
    if (height == 0) {
      return 0;
    }
    return static_cast<f32>(width) / static_cast<f32>(height);
  }
};

struct ServiceRegistered {
  std::type_index type;
  void* service;

  template <typename T>
  [[nodiscard]] bool is() const noexcept {
    return type == std::type_index(typeid(T));
  }

  template <typename T>
  [[nodiscard]] T* as() const noexcept {
    if (is<T>()) {
      return static_cast<T*>(service);
    }
    return nullptr;
  }
};

struct ServiceUnregistered {
  std::type_index type;

  template <typename T>
  [[nodiscard]] bool is() const noexcept {
    return type == std::type_index(typeid(T));
  }
};

enum class EngineEvent : u8 {
  RenderComplete,
  RenderLayerChange,
  ServiceRegistered,
  ServiceUnregistered,
  TickComplete,
  ViewportSizeChange,
};

template <typename Event>
constexpr EngineEvent getEngineEvent() noexcept;

template <>
constexpr EngineEvent getEngineEvent<RenderComplete>() noexcept {
  return EngineEvent::RenderComplete;
}

template <>
constexpr EngineEvent getEngineEvent<RenderLayerChange>() noexcept {
  return EngineEvent::RenderLayerChange;
}

template <>
constexpr EngineEvent getEngineEvent<TickComplete>() noexcept {
  return EngineEvent::TickComplete;
}

template <>
constexpr EngineEvent getEngineEvent<ViewportSizeChange>() noexcept {
  return EngineEvent::ViewportSizeChange;
}

template <>
constexpr EngineEvent getEngineEvent<ServiceRegistered>() noexcept {
  return EngineEvent::ServiceRegistered;
}

template <>
constexpr EngineEvent getEngineEvent<ServiceUnregistered>() noexcept {
  return EngineEvent::ServiceUnregistered;
}

struct EngineEventPolicies {
  template <typename Event>
  static EngineEvent getEvent(const Event& /*unused*/) {
    return getEngineEvent<Event>();
  }
};

struct EngineDispatchers {
  eventpp::EventDispatcher<EngineEvent, void(const RenderComplete&),
                           EngineEventPolicies>
      renderComplete;

  eventpp::EventDispatcher<EngineEvent, void(const ServiceRegistered&),
                           EngineEventPolicies>
      serviceRegistered;

  eventpp::EventDispatcher<EngineEvent, void(const ServiceUnregistered&),
                           EngineEventPolicies>
      serviceUnregistered;

  eventpp::EventDispatcher<EngineEvent, void(const RenderLayerChange&),
                           EngineEventPolicies>
      renderLayerChange;

  eventpp::EventDispatcher<EngineEvent, void(const TickComplete&),
                           EngineEventPolicies>
      tickComplete;

  eventpp::EventDispatcher<EngineEvent, void(const ViewportSizeChange&),
                           EngineEventPolicies>
      viewportChanged;

  template <EngineEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    if constexpr (EngineEvent::RenderComplete == E) {
      renderComplete.appendListener(E, std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::ServiceRegistered == E) {
      serviceRegistered.appendListener(E, std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::ServiceUnregistered == E) {
      serviceUnregistered.appendListener(E, std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::RenderLayerChange == E) {
      renderLayerChange.appendListener(E, std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::TickComplete == E) {
      tickComplete.appendListener(E, std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::ViewportSizeChange == E) {
      viewportChanged.appendListener(E, std::forward<Args>(args)...);
    }
  }

  template <EngineEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    if constexpr (EngineEvent::RenderComplete == E) {
      renderComplete.dispatch(std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::ServiceRegistered == E) {
      serviceRegistered.dispatch(std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::ServiceUnregistered == E) {
      serviceUnregistered.dispatch(std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::RenderLayerChange == E) {
      renderLayerChange.dispatch(std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::TickComplete == E) {
      tickComplete.dispatch(std::forward<Args>(args)...);
    } else if constexpr (EngineEvent::ViewportSizeChange == E) {
      viewportChanged.dispatch(std::forward<Args>(args)...);
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_
