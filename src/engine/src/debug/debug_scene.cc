#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"

namespace uinta {

DebugScene::DebugScene(Scene* parent) noexcept : Scene(parent) {
  addScene<DebugSceneUi>();
}

}  // namespace uinta
