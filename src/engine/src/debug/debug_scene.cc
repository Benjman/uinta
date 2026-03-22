#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"
#include "uinta/scenes/axis_viewer.h"
#include "uinta/scenes/grid.h"

namespace uinta {

DebugScene::DebugScene(Scene* parent) noexcept : Scene(parent) {
  addScene<DebugSceneUi>();
  addScene<AxisViewerScene>();
  addScene<GridScene>();
}

}  // namespace uinta
