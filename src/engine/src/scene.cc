#include "uinta/scene.h"

#include <string>

#include "absl/log/log.h"
#include "absl/status/status.h"
#include "uinta/component.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/lib/absl/status.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

Scene::Scene(Scene* parent, Engine* engine, Layer layer) noexcept
    : components_(this),
      parent_(parent),
      engine_(engine ? engine : parent_->engine()),
      runtime_(engine_->runtime()) {
  assert(engine_ && "Engine is required.");
  flags_.layer(layer);
  if (parent_ && flags_.layer() < parent_->flags_.layer()) {
    LOG(ERROR) << StrFormat(
        "Scenes with higher layer orders cannot be children of scenes with "
        "lower layer orders. The current scene has layer %s, while its "
        "parent (%s) has layer %s.",
        to_string(flags_.layer()), parent->name(), to_string(parent_->layer()));
    flags_.state(State::Error);
    return;
  }
  flags_.state(State::Running);
}

std::string to_string(Scene::State state) noexcept;

inline Status validateLayer(Scene* scene, const Scene::Layer layer) noexcept {
  assert(scene && "Scene cannot be null.");
  if (layer == scene->layer())
    return CancelledError(StrFormat("%s layer is already '%s'.", scene->name(),
                                    to_string(layer)));
  if (scene->parent()) {
    auto thisLayer = static_cast<u8>(scene->layer());
    auto parentLayer = static_cast<u8>(scene->parent()->layer());
    if (thisLayer < parentLayer)
      return CancelledError(StrFormat(
          "Scenes with higher layer orders cannot be children of scenes with "
          "lower layer orders. The current scene has layer %s, while its "
          "parent has layer %s.",
          to_string(scene->layer()), to_string(scene->parent()->layer())));
  }
  return OkStatus();
}

inline Status validateState(Scene* scene, const Scene::State state) noexcept {
  assert(scene && "Scene cannot be null.");
  if (state == scene->state()) {
    return CancelledError(StrFormat("%s state is already '%s'.", scene->name(),
                                    to_string(state)));
  }

  // TODO: Check if children's layers are of a higher order, and reject if even
  // one exists.

  return OkStatus();
}

Status Scene::layer(Layer layer) noexcept {
  if (auto status = validateLayer(this, layer); !status.ok()) {
    return status;
  }
  flags_.layer(layer);
  return parent_->onSceneLayerChange(
      SceneEvent<SceneEventType::LayerChange>(this));
}

Status Scene::state(State state) noexcept {
  if (auto status = validateState(this, state); !status.ok()) {
    return status;
  }
  flags_.state(state);
  return parent_->onSceneStateChange(
      SceneEvent<SceneEventType::StateChange>(this));
}

Status Scene::onSceneLayerChange(
    const SceneEvent<SceneEventType::LayerChange>& event) noexcept {
  LOG(INFO) << event.scene->name()
            << " layer changed: " << to_string(event.scene->layer());
  return OkStatus();
}

Status Scene::onSceneStateChange(
    const SceneEvent<SceneEventType::StateChange>& event) noexcept {
  if (this != event.scene->parent_ || this == event.scene)
    return InvalidArgumentError(
        "Mismanged attempt to handle child scene state change.");
  if (event.scene->state() == State::Complete) return removeScene(event.scene);
  LOG(INFO) << StrFormat("%s state changed to %s", event.scene->name(),
                         to_string(event.scene->state()));
  return OkStatus();
}

Status Scene::removeScene(Scene* scene) noexcept {
  if (this == scene)
    return InvalidArgumentError(
        "Attempt to remove self from scene stack detected.");
  if (sceneStack_.contains(scene)) {
    sceneStack_.remove(scene);
    scene->parent_ = nullptr;
  } else {
    if constexpr (constexpr bool ShouldCallParent = false && parent_) {
      return parent_->removeScene(scene);
    }
    std::string errorMsg =
        "Attempting to remove %s failed: Could not locate in scene stack.";
    auto childName = name();
    Scene* parent = parent_;
    while (parent) {
      errorMsg = StrCat(errorMsg, "\r\t", childName, " is a child of ",
                        parent->name());
      childName = parent->name();
      parent = parent->parent_;
    }
    return NotFoundError(errorMsg);
  }
  return OkStatus();
}

time_t Scene::runtime() const noexcept { return engine()->runtime(); }

}  // namespace uinta
