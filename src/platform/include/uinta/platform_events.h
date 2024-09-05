#ifndef SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_EVENTS_H_
#define SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_EVENTS_H_

#include <utility>

#include "eventpp/eventdispatcher.h"
#include "uinta/monitor.h"
#include "uinta/types.h"

namespace uinta {

struct OnCloseRequestEvent final {
  const void* window;
};

struct OnDebugMessageEvent final {
  u32 source;
  u32 type;
  u32 id;
  u32 severity;
  size_t length;
  const char* message;
};

struct OnErrorEvent final {
  i32 code;
  const char* description;
};

struct OnViewportSizeChangeEvent final {
  const void* window;
  f32 width;
  f32 height;
};

struct OnMonitorChangeEvent final {
  Monitor* monitor;
};

enum class PlatformEvent {
  OnCloseRequest,
  OnDebugMessage,
  OnError,
  OnMonitorChange,
  OnViewportSizeChange,
};

template <typename Event>
constexpr PlatformEvent getPlatformEvent() noexcept;

template <>
constexpr PlatformEvent getPlatformEvent<OnCloseRequestEvent>() noexcept {
  return PlatformEvent::OnCloseRequest;
}

template <>
constexpr PlatformEvent getPlatformEvent<OnDebugMessageEvent>() noexcept {
  return PlatformEvent::OnDebugMessage;
}

template <>
constexpr PlatformEvent getPlatformEvent<OnErrorEvent>() noexcept {
  return PlatformEvent::OnError;
}

template <>
constexpr PlatformEvent getPlatformEvent<OnMonitorChangeEvent>() noexcept {
  return PlatformEvent::OnMonitorChange;
}

template <>
constexpr PlatformEvent getPlatformEvent<OnViewportSizeChangeEvent>() noexcept {
  return PlatformEvent::OnViewportSizeChange;
}

struct PlatformEventPolicies {
  template <typename Event>
  static PlatformEvent getEvent(const Event&) {
    return getPlatformEvent<Event>();
  }
};

struct PlatformDispatchers {
  eventpp::EventDispatcher<PlatformEvent, void(const OnCloseRequestEvent&),
                           PlatformEventPolicies>
      onCloseRequest;

  eventpp::EventDispatcher<PlatformEvent, void(const OnDebugMessageEvent&),
                           PlatformEventPolicies>
      onDebugMessage;

  eventpp::EventDispatcher<PlatformEvent, void(const OnErrorEvent&),
                           PlatformEventPolicies>
      onError;

  eventpp::EventDispatcher<PlatformEvent, void(const OnMonitorChangeEvent&),
                           PlatformEventPolicies>
      onMonitorChange;

  eventpp::EventDispatcher<PlatformEvent,
                           void(const OnViewportSizeChangeEvent&),
                           PlatformEventPolicies>
      onViewportSizeChange;

  template <PlatformEvent E, typename... Args>
  void addListener(Args&&... args) noexcept {
    if constexpr (PlatformEvent::OnCloseRequest == E)
      onCloseRequest.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnDebugMessage == E)
      onDebugMessage.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnError == E)
      onError.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnMonitorChange == E)
      onMonitorChange.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnViewportSizeChange == E)
      onViewportSizeChange.appendListener(E, std::forward<Args>(args)...);
  }

  template <PlatformEvent E, typename... Args>
  void dispatch(Args&&... args) const noexcept {
    if constexpr (PlatformEvent::OnCloseRequest == E)
      onCloseRequest.dispatch(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnDebugMessage == E)
      onDebugMessage.dispatch(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnError == E)
      onError.dispatch(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnMonitorChange == E)
      onMonitorChange.dispatch(E, std::forward<Args>(args)...);
    else if constexpr (PlatformEvent::OnViewportSizeChange == E)
      onViewportSizeChange.dispatch(E, std::forward<Args>(args)...);
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_PLATFORM_EVENTS_H_
