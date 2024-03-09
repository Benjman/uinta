#include "uinta/scene.h"

#include <string>

#include "absl/log/log.h"
#include "uinta/engine.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

std::string to_string(Scene::State state) noexcept;

Scene::Scene(Layer layer) noexcept {
  flags_ |= static_cast<SceneFlag>(layer) << LayerShift;
  flags_ |= static_cast<SceneFlag>(State::Running) << StateShift;
}

Scene::Scene(Scene&& other) noexcept
    : components_(std::move(other.components_)),
      scenes_(std::move(other.scenes_)),
      flags_(other.flags_),
      engine_(other.engine_),
      parent_(other.parent_) {
  other.flags_ = 0;
  other.engine_ = nullptr;
  other.parent_ = nullptr;
}

Scene& Scene::operator=(Scene&& other) noexcept {
  if (this != &other) {
    flags_ = other.flags_;
    other.flags_ = 0;

    engine_ = other.engine_;
    other.engine_ = nullptr;

    parent_ = other.parent_;
    other.parent_ = nullptr;

    components_ = std::move(other.components_);
    scenes_ = std::move(other.scenes_);
  }
  return *this;
}

Status Scene::state(State state) noexcept {
  if (auto status = validNewState(state); !status.ok()) {
    if (IsAlreadyExists(status)) {
      LOG(WARNING) << status.message();
      return {};
    }
    return status;
  }
  flags_ &= ~StateMask;
  flags_ |= static_cast<SceneFlag>(state) << StateShift;
  return {};
}

Status Scene::validNewState(State state) const noexcept {
  if (state == this->state())
    return AlreadyExistsError(
        StrFormat("Scene state is already '%s'.", to_string(state)));
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
