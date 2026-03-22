#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_EVENTS_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_EVENTS_H_

#include <eventpp/eventdispatcher.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <utility>

#include "uinta/types.h"

namespace uinta {

class Camera;

enum class CameraEvent : u8 {
  ViewMatrixUpdated,
};

struct ViewMatrixUpdateEvent final {
  const Camera* camera;
  glm::mat4 view;
};

template <typename Event>
constexpr CameraEvent getCameraEvent() noexcept;

template <>
constexpr CameraEvent getCameraEvent<ViewMatrixUpdateEvent>() noexcept {
  return CameraEvent::ViewMatrixUpdated;
}

struct CameraEventPolicies {
  template <typename Event>
  static CameraEvent getEvent(const Event& /*unused*/) {
    return getCameraEvent<Event>();
  }
};

struct CameraDispatchers {
  eventpp::EventDispatcher<CameraEvent, void(const ViewMatrixUpdateEvent&),
                           CameraEventPolicies>
      viewMatrixUpdate;

  template <CameraEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    if constexpr (CameraEvent::ViewMatrixUpdated == E) {
      viewMatrixUpdate.appendListener(E, std::forward<Args>(args)...);
    }
  }

  template <CameraEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    if constexpr (CameraEvent::ViewMatrixUpdated == E) {
      viewMatrixUpdate.dispatch(std::forward<Args>(args)...);
    }
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_EVENTS_H_
