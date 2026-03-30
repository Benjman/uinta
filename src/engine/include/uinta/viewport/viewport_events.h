#ifndef SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_EVENTS_H_
#define SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_EVENTS_H_

#include <eventpp/eventdispatcher.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <utility>

#include "uinta/types.h"

namespace uinta {

class ViewportManager;

enum class ViewportEvent : u8 {
  ProjectionMatrixUpdated,
};

struct ProjectionMatrixUpdateEvent final {
  const ViewportManager* viewport;
  glm::mat4 projection;
  f32 aspect;
};

template <typename Event>
constexpr ViewportEvent getViewportEvent() noexcept;

template <>
constexpr ViewportEvent
getViewportEvent<ProjectionMatrixUpdateEvent>() noexcept {
  return ViewportEvent::ProjectionMatrixUpdated;
}

struct ViewportEventPolicies {
  template <typename Event>
  static ViewportEvent getEvent(const Event& /*unused*/) {
    return getViewportEvent<Event>();
  }
};

struct ViewportDispatchers {
  eventpp::EventDispatcher<ViewportEvent,
                           void(const ProjectionMatrixUpdateEvent&),
                           ViewportEventPolicies>
      projectionMatrixUpdate;

  template <ViewportEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    if constexpr (ViewportEvent::ProjectionMatrixUpdated == E) {
      projectionMatrixUpdate.appendListener(E, std::forward<Args>(args)...);
    }
  }

  template <ViewportEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    if constexpr (ViewportEvent::ProjectionMatrixUpdated == E) {
      projectionMatrixUpdate.dispatch(E, std::forward<Args>(args)...);
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_EVENTS_H_
