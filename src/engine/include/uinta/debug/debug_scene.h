#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_

#include "uinta/scene/scene.h"

namespace uinta {

class DebugScene : public Scene {
 public:
  explicit DebugScene(Engine*) noexcept;
  explicit DebugScene(Scene*) noexcept;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_
