#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_EVENTS_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_EVENTS_H_

#include <utility>

#include "eventpp/eventdispatcher.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "uinta/types.h"

namespace uinta {

class Camera;

enum class CameraEvent {
  ClippingPlanesChange,
  ViewMatrixUpdated,
};

struct ClippingPlanesChangeEvent final {
  const Camera* camera;
  f32 newNear;
  f32 newFar;
  f32 oldNear;
  f32 oldFar;
};

struct ViewMatrixUpdateEvent final {
  const Camera* camera;
  glm::mat4 view;
};

template <typename Event>
constexpr CameraEvent getCameraEvent() noexcept;

template <>
constexpr CameraEvent getCameraEvent<ClippingPlanesChangeEvent>() noexcept {
  return CameraEvent::ClippingPlanesChange;
}

template <>
constexpr CameraEvent getCameraEvent<ViewMatrixUpdateEvent>() noexcept {
  return CameraEvent::ViewMatrixUpdated;
}

struct CameraEventPolicies {
  template <typename Event>
  static CameraEvent getEvent(const Event&) {
    return getCameraEvent<Event>();
  }
};

struct CameraDispatchers {
  eventpp::EventDispatcher<CameraEvent, void(const ClippingPlanesChangeEvent&),
                           CameraEventPolicies>
      clippingPlanesChange;

  eventpp::EventDispatcher<CameraEvent, void(const ViewMatrixUpdateEvent&),
                           CameraEventPolicies>
      viewMatrixUpdate;

  template <CameraEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    if constexpr (CameraEvent::ClippingPlanesChange == E)
      clippingPlanesChange.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (CameraEvent::ViewMatrixUpdated == E)
      viewMatrixUpdate.appendListener(E, std::forward<Args>(args)...);
  }

  template <CameraEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    if constexpr (CameraEvent::ClippingPlanesChange == E)
      clippingPlanesChange.dispatch(std::forward<Args>(args)...);
    else if constexpr (CameraEvent::ViewMatrixUpdated == E)
      viewMatrixUpdate.dispatch(std::forward<Args>(args)...);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_CAMERA_EVENTS_H_
