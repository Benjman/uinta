#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"
#include "uinta/scenes/axis_viewer.h"
#include "uinta/scenes/grid.h"

namespace uinta {

DebugScene::DebugScene(Scene* parent, Engine* engine,
                       const DebugSceneParams* params) noexcept
    : Scene(parent, engine, parent->layer()) {
  addScene<DebugSceneUi>(params);
  addScene<AxisViewerScene>(params->camera);
  addScene<GridScene>(15, params->camera);
}

}  // namespace uinta
