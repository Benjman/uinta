#include "uinta/scene/scene_system.h"

#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include <algorithm>
#include <cassert>

#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"

namespace uinta {

namespace {

bool containsLayer(const Scene* scene, SceneLayer layer) noexcept {
  return scene->layer() == layer ||
         std::any_of(scene->children().begin(), scene->children().end(),
                     [layer](const auto& child) { return containsLayer(child.get(), layer); });
}

}  // namespace

SceneSystem::SceneSystem(Engine* engine) noexcept : engine_(engine) {
  dispatchers_.addListener<SceneEvent::StateChange>([this](const SceneStateChangeEvent& event) {
    if (event.newState == SceneState::Complete || event.newState == SceneState::Error) {
      if (std::ranges::find(pendingRemoval_, event.scene) == pendingRemoval_.end()) {
        pendingRemoval_.push_back(event.scene);
      }
    } else {
      // e.g. a scene was completed, then resumed before the next flush.
      forget(event.scene);
    }
  });
}

SceneSystem::~SceneSystem() noexcept { roots_.clear(); }

void SceneSystem::flush() noexcept {
  while (!pendingRemoval_.empty()) {
    const auto* scene = pendingRemoval_.back();
    pendingRemoval_.pop_back();
    remove(scene);
  }
}

void SceneSystem::forget(const Scene* scene) noexcept { std::erase(pendingRemoval_, scene); }

void SceneSystem::remove(const Scene* scene) noexcept {
  assert(scene && "Scene cannot be null.");
  if (auto* parent = const_cast<Scene*>(scene->parent())) {
    if (!parent->children_.contains(scene)) {
      LOG(WARNING) << absl::StrFormat("%s could not be located within the children of %s.", scene->name(),
                                      parent->name());
      return;
    }
    dispatchers_.dispatch<SceneEvent::SceneRemoved>(SceneRemovedEvent(scene));
    parent->children_.remove(scene);
    return;
  }

  auto itr = std::ranges::find_if(roots_, [scene](const auto& root) { return root.get() == scene; });
  if (itr == roots_.end()) {
    // Root scenes not owned by the system (e.g. stack allocated) are left to
    // their owner.
    return;
  }
  dispatchers_.dispatch<SceneEvent::SceneRemoved>(SceneRemovedEvent(scene));
  roots_.erase(itr);
}

void SceneSystem::preTick(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->preTick(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { preTick(scene.get(), delta); });
  }
}

void SceneSystem::tick(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->tick(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { tick(scene.get(), delta); });
  }
}

void SceneSystem::postTick(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isTicking()) {
    scene->postTick(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { postTick(scene.get(), delta); });
  }
}

void SceneSystem::preRender(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isRendering()) {
    scene->preRender(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { preRender(scene.get(), delta); });
  }
}

void SceneSystem::render(Scene* scene, time_t delta) noexcept {
  assert(scene);
  static_assert(SceneLayers.front() == SceneLayer::Simulation);
  std::ranges::for_each(SceneLayers.begin(), SceneLayers.end(), [this, scene, delta](auto layer) {
    if (layer != SceneLayers.front() && containsLayer(scene, layer)) {
      engine_->dispatchers()->dispatch<EngineEvent::RenderLayerChange>(RenderLayerChange(layer));
    }
    renderLayer(scene, layer, delta);
  });
}

void SceneSystem::renderLayer(Scene* scene, SceneLayer layer, time_t delta) noexcept {
  assert(scene);
  if (scene->isRendering()) {
    if (scene->layer() == layer) {
      scene->render(delta);
    }
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, layer, delta](auto& scene) { renderLayer(scene.get(), layer, delta); });
  }
}

void SceneSystem::postRender(Scene* scene, time_t delta) noexcept {
  assert(scene);
  if (scene->isRendering()) {
    scene->postRender(delta);
    std::for_each(scene->children().begin(), scene->children().end(),
                  [this, delta](auto& scene) { postRender(scene.get(), delta); });
  }
}

}  // namespace uinta
