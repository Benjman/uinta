#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_

#include <utility>

#include "eventpp/eventdispatcher.h"
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

  f32 aspect() const noexcept {
    if (height == 0) return 0;
    return static_cast<f32>(width) / static_cast<f32>(height);
  }
};

enum class EngineEvent {
  RenderComplete,
  RenderLayerChange,
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

struct EngineEventPolicies {
  template <typename Event>
  static EngineEvent getEvent(const Event&) {
    return getEngineEvent<Event>();
  }
};

struct EngineDispatchers {
  eventpp::EventDispatcher<EngineEvent, void(const RenderComplete&),
                           EngineEventPolicies>
      renderComplete;

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
    if constexpr (EngineEvent::RenderComplete == E)
      renderComplete.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (EngineEvent::RenderLayerChange == E)
      renderLayerChange.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (EngineEvent::TickComplete == E)
      tickComplete.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (EngineEvent::ViewportSizeChange == E)
      viewportChanged.appendListener(E, std::forward<Args>(args)...);
  }

  template <EngineEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    if constexpr (EngineEvent::RenderComplete == E)
      renderComplete.dispatch(std::forward<Args>(args)...);
    else if constexpr (EngineEvent::RenderLayerChange == E)
      renderLayerChange.dispatch(std::forward<Args>(args)...);
    else if constexpr (EngineEvent::TickComplete == E)
      tickComplete.dispatch(std::forward<Args>(args)...);
    else if constexpr (EngineEvent::ViewportSizeChange == E)
      viewportChanged.dispatch(std::forward<Args>(args)...);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_
