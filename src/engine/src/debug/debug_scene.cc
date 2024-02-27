#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"
#include "uinta/scenes/axis_viewer.h"

namespace uinta {

DebugScene::DebugScene(Engine* engine) noexcept : Scene(engine, SCENE_NAME) {
  addScene<DebugSceneUi>();
  addScene<AxisViewerScene>();
}

DebugScene::DebugScene(Scene* parent) noexcept
    : DebugScene(parent ? parent->engine() : nullptr) {}

}  // namespace uinta
