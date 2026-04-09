#include "uinta/scene/scene.h"

#include <absl/log/log.h>
#include <absl/status/status.h>
#include <absl/strings/str_format.h>

#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/scene/scene_events.h"
#include "uinta/scene/scene_system.h"
#include "uinta/status.h"

namespace uinta {

Scene::Scene(Engine* engine, SceneLayer layer, SceneState state) noexcept : engine_(engine) {
  flags_.layer(layer);
  flags_.state(state);
}

Scene::Scene(Scene* parent, SceneLayer layer, SceneState state) noexcept : Scene(parent->engine(), layer, state) {
  assert(parent && "`Scene*` cannot be null.");
  parent_ = parent;
}

inline Status validateLayer(Scene* scene, const SceneLayer layer) noexcept {
  assert(scene && "Scene cannot be null.");
  if (layer == scene->layer()) {
    return CancelledError(absl::StrFormat("%s layer is already '%s'.", scene->name(), to_string(layer)));
  }
  if (scene->parent() != nullptr) {
    auto thisLayer = static_cast<u8>(scene->layer());
    auto parentLayer = static_cast<u8>(scene->parent()->layer());
    if (thisLayer < parentLayer) {
      return CancelledError(
          absl::StrFormat("Scenes with higher layer orders cannot be children of scenes with lower layer orders. The "
                          "current scene has layer %s, while its parent has layer %s.",
                          to_string(scene->layer()), to_string(scene->parent()->layer())));
    }
  }
  return OkStatus();
}

inline Status validateState(Scene* scene, const SceneState state) noexcept {
  assert(scene && "Scene cannot be null.");
  if (state == scene->state()) {
    return CancelledError(absl::StrFormat("%s state is already '%s'.", scene->name(), to_string(state)));
  }

  // TODO: Check if children's layers are of a higher order, and reject if even
  // one exists.

  return OkStatus();
}

void Scene::layer(SceneLayer layer) noexcept {
  if (auto status = validateLayer(this, layer); !status.ok()) {
    LOG(WARNING) << status.message();
    return;
  }
  auto prevLayer = flags_.layer();
  flags_.layer(layer);
  if (auto* system = sceneSystem()) {
    system->dispatchers()->dispatch<SceneEvent::LayerChange>(SceneLayerChangeEvent(this, prevLayer, layer));
  }
}

void Scene::state(SceneState state) noexcept {
  if (auto status = validateState(this, state); !status.ok()) {
    LOG(WARNING) << status.message();
    return;
  }
  auto prevState = flags_.state();
  flags_.state(state);
  if (auto* system = sceneSystem()) {
    system->dispatchers()->dispatch<SceneEvent::StateChange>(SceneStateChangeEvent(this, prevState, state));
  }
}

Scene::~Scene() noexcept {
  if (auto* system = sceneSystem()) {
    system->forget(this);
  }
}

SceneSystem* Scene::sceneSystem() noexcept {
  auto* engine = this->engine();
  return engine != nullptr ? engine->scenes() : nullptr;
}

void Scene::notifyAdded(const Scene* scene) noexcept {
  if (auto* system = sceneSystem()) {
    system->dispatchers()->dispatch<SceneEvent::SceneAdded>(SceneAddedEvent(scene));
  }
}

time_t Scene::runtime() const noexcept { return engine()->runtime(); }

}  // namespace uinta
