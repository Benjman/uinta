#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine.h"
#include "uinta/utils/viewport_change.h"

namespace uinta {

DebugScene::DebugScene(Params params, Layer layer) noexcept
    : Scene(params.engine, layer),
      shader_(params.gl),
      gl_(params.gl),
      params_(params) {
  params_.shader = &shader_;
  addScene<DebugSceneUi>(this, params_);
}

void DebugScene::onViewportSizeChange(
    const ViewportSizeChange& event) noexcept {
  ShaderGuard sg(&shader_);
  shader_.projection = glm::perspective(45.0f, event.aspect(), 0.001f, 10.0f);
}

void DebugScene::render(const EngineState& state) noexcept {
  Scene::render(state);
  ShaderGuard sg(&shader_);
  shader_.time = state.runtime();
}

}  // namespace uinta
