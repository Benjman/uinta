#include "uinta/shaders/basic_shader.h"

#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"

namespace uinta {

BasicShaderManager::BasicShaderManager(Scene* scene) noexcept
    : shader_(scene->engine()->gl()) {}

}  // namespace uinta
