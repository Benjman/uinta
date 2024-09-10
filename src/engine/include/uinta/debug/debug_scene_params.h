#ifndef SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_PARAMS_H_
#define SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_PARAMS_H_

#include <array>

#include "glm/ext/vector_float4.hpp"

namespace uinta {

using Palette = std::array<glm::vec4, 5>;

class BasicShader;
class ComponentManager;
class SystemManager;

struct DebugSceneParams {
  Palette palette;
  BasicShader* shader;
  ComponentManager* components;
  SystemManager* systems;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_DEBUG_DEBUG_SCENE_PARAMS_H_
