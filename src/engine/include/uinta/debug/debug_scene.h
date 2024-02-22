#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_

#include <string>

#include "uinta/scene.h"

namespace uinta {

class Camera;
class PrimitiveShader;

struct DebugSceneParams {
  Camera* camera;
  PrimitiveShader* shader;
};

class DebugScene : public Scene {
 public:
  explicit DebugScene(Scene*, Engine*, const DebugSceneParams*) noexcept;

  std::string name() const noexcept override { return "DebugScene"; }
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_H_
