#include "uinta/shaders/basic_shader.h"

#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"

namespace uinta {

BasicShaderManager::BasicShaderManager(Scene* scene) noexcept
    : shader_(scene->engine()->gl()) {
  scene->engine()->dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
      [this](const auto& event) {
        projection_ = glm::perspective(glm::radians<f32>(45), event.aspect(),
                                       0.01f, 4.0f);
        flags_.isProjectionDirty(true);
      });
}

}  // namespace uinta
