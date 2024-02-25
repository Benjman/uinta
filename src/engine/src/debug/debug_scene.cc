#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"
#include "uinta/scenes/axis_viewer.h"
#include "uinta/scenes/grid.h"

namespace uinta {

DebugScene::DebugScene(Engine* engine) noexcept : Scene(engine, SCENE_NAME) {
  addScene<DebugSceneUi>();
  addScene<AxisViewerScene>();
  addScene<GridScene>();
}

DebugScene::DebugScene(Scene* parent) noexcept
    : DebugScene(parent ? parent->engine() : nullptr) {}

}  // namespace uinta
