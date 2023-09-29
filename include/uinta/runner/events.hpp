#ifndef UINTA_RUNNER_EVENTS_HPP
#define UINTA_RUNNER_EVENTS_HPP

#include <uinta/event.hpp>

namespace uinta {

enum class RunnerEvents : event_t {
  AspectRatioChanged,
  SceneCreated,
  SceneDestroyed,
  ScenePaused,
  SceneRunning,
};

struct AspectRatioChangeEvent : Event {
  const f64 aspect_ratio;

  AspectRatioChangeEvent(f64 at, f32 aspect_ratio) : Event(at), aspect_ratio(aspect_ratio) {
  }
};

struct SceneEvent : Event {
  const Scene* scene;

  SceneEvent(f64 at, const Scene* scene) : Event(at), scene(scene) {
  }
};

}  // namespace uinta

#endif  // UINTA_RUNNER_EVENTS_HPP
