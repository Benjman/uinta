#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"
#include "uinta/scenes/axis_viewer.h"

namespace uinta {

DebugScene::DebugScene(Scene* parent) noexcept : Scene(parent) {
  addScene<DebugSceneUi>();
  addScene<AxisViewerScene>();
}

}  // namespace uinta
