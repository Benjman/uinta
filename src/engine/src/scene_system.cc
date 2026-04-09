#include "uinta/scene/scene_system.h"

#include <algorithm>
#include <cassert>

#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"

namespace uinta {

namespace {

// Destroys `scenes` bottom-up, last-added first. A scene's descendants are
// always destroyed before its own destructor runs, so they never outlive the
// members (e.g. services) of the scene that owns them.
template <typename Container>
void destroy(Container& scenes) noexcept {
  while (!scenes.empty()) {
    destroy(scenes.back()->children());
    scenes.pop_back();
  }
}

template <typename Container>
void prune(Container& scenes) noexcept {
  for (auto& scene : scenes) {
    if (scene->isComplete()) {
      destroy(scene->children());
    }
  }
  std::erase_if(scenes, [](const auto& scene) { return scene->isComplete(); });
  for (auto& scene : scenes) {
    prune(scene->children());
  }
}

}  // namespace

SceneSystem::SceneSystem(Engine* engine) noexcept : serviceHandle_(engine, this), engine_(engine) {}

SceneSystem::~SceneSystem() noexcept { destroy(roots_); }

void SceneSystem::flush() noexcept { prune(roots_); }

void SceneSystem::render(Scene* scene, time_t delta) noexcept {
  assert(scene);
  visit<&Scene::render>(scene, delta);
  engine_->dispatchers()->dispatch<EngineEvent::SimulationRendered>(SimulationRendered{});
  visit<&Scene::renderUi>(scene, delta);
  visit<&Scene::renderDebug>(scene, delta);
}

}  // namespace uinta
