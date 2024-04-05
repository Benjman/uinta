#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_

#include <utility>

#include "eventpp/eventdispatcher.h"
#include "uinta/types.h"

namespace uinta {

class EngineState;
class Scene;

struct RenderComplete {
  const EngineState* state;
  time_t duration;
};

struct SceneAddedEvent {
  const Scene* scene;
};

struct TickComplete {
  const EngineState* state;
  time_t duration;
};

struct ViewportSizeChange {
  u32 width;
  u32 height;

  f32 aspect() const noexcept {
    return static_cast<f32>(width) / static_cast<f32>(height);
  }
};

enum class EngineEvent {
  RenderComplete,
  SceneAdded,
  TickComplete,
  ViewportSizeChange,
};

template <typename Event>
constexpr EngineEvent getEngineEvent() noexcept;

template <>
constexpr EngineEvent getEngineEvent<SceneAddedEvent>() noexcept {
  return EngineEvent::SceneAdded;
}

template <>
constexpr EngineEvent getEngineEvent<RenderComplete>() noexcept {
  return EngineEvent::RenderComplete;
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

  eventpp::EventDispatcher<EngineEvent, void(const SceneAddedEvent&),
                           EngineEventPolicies>
      sceneAdded;

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
    else if constexpr (EngineEvent::SceneAdded == E)
      sceneAdded.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (EngineEvent::TickComplete == E)
      tickComplete.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (EngineEvent::ViewportSizeChange == E)
      viewportChanged.appendListener(E, std::forward<Args>(args)...);
  }

  template <EngineEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    if constexpr (EngineEvent::RenderComplete == E)
      renderComplete.dispatch(std::forward<Args>(args)...);
    else if constexpr (EngineEvent::SceneAdded == E)
      sceneAdded.dispatch(std::forward<Args>(args)...);
    else if constexpr (EngineEvent::TickComplete == E)
      tickComplete.dispatch(std::forward<Args>(args)...);
    else if constexpr (EngineEvent::ViewportSizeChange == E)
      viewportChanged.dispatch(std::forward<Args>(args)...);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_EVENTS_H_
