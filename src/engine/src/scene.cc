#include "uinta/scene.h"

#include <string>

#include "absl/log/log.h"
#include "uinta/engine.h"
#include "uinta/lib/absl/status.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"

namespace uinta {

Scene::Scene(Scene* parent, Layer layer) noexcept
    : Scene(parent ? parent->engine() : static_cast<const Engine*>(nullptr),
            layer) {
  parent_ = parent;
}

Scene::Scene(const Engine* engine, Layer layer) noexcept
    : engine_(engine),
      startedOn_(engine ? engine->platform()->runtime().value() : 0) {
  flags_ |= static_cast<SceneFlag>(layer) << LayerShift;
  flags_ |= static_cast<SceneFlag>(State::Running) << StateShift;
}

std::string to_string(Scene::State state) noexcept;

inline Status validateState(Scene* scene, const Scene::State state) noexcept {
  if (!scene) return InvalidArgumentError("Scene can not be null.");
  if (state == scene->state())
    return AlreadyExistsError(
        StrFormat("Scene state is already '%s'.", to_string(state)));
  return {};
}

Status Scene::state(State state) noexcept {
  if (auto status = validateState(this, state); !status.ok()) {
    if (IsAlreadyExists(status)) {
      LOG(WARNING) << status.message();
      return {};
    }
    return status;
  }
  flags_ &= ~StateMask;
  flags_ |= static_cast<SceneFlag>(state) << StateShift;
  if (this->state() == State::Complete && parent_) parent_->removeScene(this);
  return {};
}

std::string to_string(Scene::State state) noexcept {
  switch (state) {
    case Scene::State::Complete:
      return "Complete";
    case Scene::State::Error:
      return "Error";
    case Scene::State::Pause:
      return "Pause";
    case Scene::State::Running:
      return "Running";
    default:
      LOG(FATAL) << "Unknown Scene State";
  }
}

}  // namespace uinta
