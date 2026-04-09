#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_LAYER_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_LAYER_H_

#include <array>
#include <string>

#include "uinta/types.h"

namespace uinta {

enum class SceneLayer : u8 {
  Simulation,
  UI,
  Debug,
};

static constexpr std::array<SceneLayer, 3> SceneLayers = {
    SceneLayer::Simulation,
    SceneLayer::UI,
    SceneLayer::Debug,
};

inline std::string to_string(SceneLayer layer) noexcept {
  switch (layer) {
    case SceneLayer::Simulation:
      return "SceneLayer::Simulation";
    case SceneLayer::UI:
      return "SceneLayer::UI";
    case SceneLayer::Debug:
      return "SceneLayer::Debug";
    default:
      return "Unknown scene layer";
  }
}

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENE_SCENE_LAYER_H_
