#include "uinta/debug/debug_scene.h"

#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_events.h"

namespace uinta {

DebugScene::DebugScene(Scene* parent, Engine* engine,
                       const DebugSceneParams* params) noexcept
    : Scene(parent, engine, parent->layer()) {
  addScene<DebugSceneUi>(params);
}

}  // namespace uinta
