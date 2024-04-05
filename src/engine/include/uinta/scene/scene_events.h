#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_EVENTS_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_EVENTS_H_

#include <utility>

#include "eventpp/eventdispatcher.h"
#include "uinta/scene/scene_layer.h"
#include "uinta/scene/scene_state.h"

namespace uinta {

class Scene;

struct SceneAddedEvent final {
  const Scene* scene;
};

struct SceneRemovedEvent final {
  const Scene* scene;
};

struct SceneStateChangeEvent final {
  const Scene* scene;
  SceneState oldState;
  SceneState newState;
};

struct SceneLayerChangeEvent final {
  const Scene* scene;
  SceneLayer oldLayer;
  SceneLayer newLayer;
};

enum class SceneEvent {
  SceneAdded,
  SceneRemoved,
  StateChange,
  LayerChange,
};

template <typename Event>
constexpr SceneEvent getSceneEvent() noexcept;

template <>
constexpr SceneEvent getSceneEvent<SceneAddedEvent>() noexcept {
  return SceneEvent::SceneAdded;
}

template <>
constexpr SceneEvent getSceneEvent<SceneRemovedEvent>() noexcept {
  return SceneEvent::SceneRemoved;
}

template <>
constexpr SceneEvent getSceneEvent<SceneStateChangeEvent>() noexcept {
  return SceneEvent::StateChange;
}

template <>
constexpr SceneEvent getSceneEvent<SceneLayerChangeEvent>() noexcept {
  return SceneEvent::LayerChange;
}

struct SceneEventPolicies {
  template <typename Event>
  static SceneEvent getEvent(const Event&) {
    return getSceneEvent<Event>();
  }
};

struct SceneDispatchers {
  eventpp::EventDispatcher<SceneEvent, void(const SceneAddedEvent&),
                           SceneEventPolicies>
      added;

  eventpp::EventDispatcher<SceneEvent, void(const SceneRemovedEvent&),
                           SceneEventPolicies>
      removed;

  eventpp::EventDispatcher<SceneEvent, void(const SceneStateChangeEvent&),
                           SceneEventPolicies>
      stateChange;

  eventpp::EventDispatcher<SceneEvent, void(const SceneLayerChangeEvent&),
                           SceneEventPolicies>
      layerChange;

  template <SceneEvent E, typename... Args>
  auto addListener(Args&&... args) noexcept {
    if constexpr (SceneEvent::SceneAdded == E)
      added.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (SceneEvent::SceneRemoved == E)
      removed.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (SceneEvent::StateChange == E)
      stateChange.appendListener(E, std::forward<Args>(args)...);
    else if constexpr (SceneEvent::LayerChange == E)
      layerChange.appendListener(E, std::forward<Args>(args)...);
  }

  template <SceneEvent E, typename... Args>
  auto dispatch(Args&&... args) const noexcept {
    if constexpr (SceneEvent::SceneAdded == E)
      added.dispatch(std::forward<Args>(args)...);
    else if constexpr (SceneEvent::SceneAdded == E)
      removed.dispatch(std::forward<Args>(args)...);
    else if constexpr (SceneEvent::StateChange == E)
      stateChange.dispatch(std::forward<Args>(args)...);
    else if constexpr (SceneEvent::LayerChange == E)
      layerChange.dispatch(std::forward<Args>(args)...);
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_EVENTS_H_
