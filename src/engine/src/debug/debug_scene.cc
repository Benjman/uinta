#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"

namespace uinta {

DebugScene::DebugScene(Engine* engine) noexcept : Scene(engine, SCENE_NAME) {
  addScene<DebugSceneUi>();
}

DebugScene::DebugScene(Scene* parent) noexcept
    : DebugScene(parent ? parent->engine() : nullptr) {}

}  // namespace uinta
