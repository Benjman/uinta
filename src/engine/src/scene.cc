#include "uinta/scene/scene.h"

#include "absl/log/log.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "uinta/component.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/reflect.h"
#include "uinta/scene/scene_events.h"
#include "uinta/status.h"

namespace uinta {

Scene::Scene(Engine* engine, std::string name, SceneLayer layer) noexcept
    : name_(demangleTypeName(name)), components_(this), engine_(engine) {
  flags_.layer(layer);
  flags_.state(SceneState::Running);
}

Scene::Scene(Scene* parent, std::string name, SceneLayer layer) noexcept
    : Scene(parent->engine(), name, layer) {
  parent_ = parent;
}

inline Status validateLayer(Scene* scene, const SceneLayer layer) noexcept {
  assert(scene && "Scene cannot be null.");
  if (layer == scene->layer())
    return CancelledError(absl::StrFormat("%s layer is already '%s'.",
                                          scene->name(), to_string(layer)));
  if (scene->parent()) {
    auto thisLayer = static_cast<u8>(scene->layer());
    auto parentLayer = static_cast<u8>(scene->parent()->layer());
    if (thisLayer < parentLayer)
      return CancelledError(absl::StrFormat(
          "Scenes with higher layer orders cannot be children of scenes with "
          "lower layer orders. The current scene has layer %s, while its "
          "parent has layer %s.",
          to_string(scene->layer()), to_string(scene->parent()->layer())));
  }
  return OkStatus();
}

inline Status validateState(Scene* scene, const SceneState state) noexcept {
  assert(scene && "Scene cannot be null.");
  if (state == scene->state())
    return CancelledError(absl::StrFormat("%s state is already '%s'.",
                                          scene->name(), to_string(state)));

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
  dispatchers_.dispatch<SceneEvent::LayerChange>(
      SceneLayerChangeEvent(this, prevLayer, layer));
}

void Scene::state(SceneState state) noexcept {
  if (auto status = validateState(this, state); !status.ok()) {
    LOG(WARNING) << status.message();
    return;
  }
  auto prevState = flags_.state();
  flags_.state(state);
  dispatchers_.dispatch<SceneEvent::StateChange>(
      SceneStateChangeEvent(this, prevState, state));
}

Status Scene::onSceneLayerChange(const SceneLayerChangeEvent& event) noexcept {
  LOG(INFO) << absl::StrFormat("%s layer changed to %s", event.scene->name(),
                               to_string(event.scene->layer()));
  return status_;
}

Status Scene::onSceneStateChange(const SceneStateChangeEvent& event) noexcept {
  if (this != event.scene->parent_ || this == event.scene)
    return InvalidArgumentError(
        "Mismanged attempt to handle child scene state change.");
  if (event.scene->state() == SceneState::Complete) {
    auto itr = std::find_if(
        children_.begin(), children_.end(),
        [&event](const auto& scene) { return scene.get() == event.scene; });
    if (itr == children_.end())
      return InternalError(
          "Failed to locate scene within children collection.");
    return removeScene(itr->get());
  }
  LOG(INFO) << absl::StrFormat("%s state changed to %s", event.scene->name(),
                               to_string(event.scene->state()));
  return status_;
}

Status Scene::removeScene(Scene* scene) noexcept {
  if (this == scene)
    return InvalidArgumentError(
        "Attempt to remove self from scene stack detected.");
  if (children_.contains(scene)) {
    children_.remove(scene);
    dispatchers_.dispatch<SceneEvent::SceneRemoved>(SceneRemovedEvent(scene));
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
      errorMsg = absl::StrCat(errorMsg, "\r\t", childName, " is a child of ",
                              parent->name());
      childName = parent->name();
      parent = parent->parent_;
    }
    return NotFoundError(errorMsg);
  }
  return status_;
}

time_t Scene::runtime() const noexcept { return engine()->runtime(); }

}  // namespace uinta
