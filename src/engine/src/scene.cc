#include "uinta/scene.h"

#include <string>

#include "absl/log/log.h"
#include "uinta/engine.h"
#include "uinta/lib/absl/strings.h"

namespace uinta {

std::string to_string(Scene::State state) noexcept;

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
