#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_STATE_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_STATE_H_

#include <array>
#include <string>

#include "uinta/types.h"

namespace uinta {

enum class SceneState : u8 {
  Running,
  Pause,
  Complete,
};

static constexpr std::array<SceneState, 3> SceneStates = {
    SceneState::Running,
    SceneState::Pause,
    SceneState::Complete,
};

inline std::string to_string(SceneState state) noexcept {
  switch (state) {
    case SceneState::Running:
      return "SceneState::Running";
    case SceneState::Pause:
      return "SceneState::Pause";
    case SceneState::Complete:
      return "SceneState::Complete";
    default:
      return "Unknown scene state";
  }
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_STATE_H_
