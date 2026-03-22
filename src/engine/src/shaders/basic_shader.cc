#include "uinta/shaders/basic_shader.h"

#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"
#include "uinta/viewport/viewport_manager.h"

namespace uinta {

BasicShaderManager::BasicShaderManager(Engine* engine) noexcept
    : serviceHandle_(engine, this), shader_(engine->gl()) {
  auto* viewportManager = engine->service<ViewportManager>();
  assert(viewportManager);
  viewportManager->addListener<ViewportEvent::ProjectionMatrixUpdated>(
      [this](const auto& event) {
        projection_ = event.projection;
        flags_.isProjectionDirty(true);
      });
  ShaderGuard shaderGuard(&shader_);
  view(glm::mat4(1));
  projection(glm::mat4(1));
  model(glm::mat4(1));
}

}  // namespace uinta
