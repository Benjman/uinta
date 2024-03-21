#ifndef SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_

#include "uinta/camera/camera_manager.h"
#include "uinta/debug/debug_scene.h"
#include "uinta/engine/cursor_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/scene/scene.h"
#include "uinta/scenes/demo_palette.h"
#include "uinta/scenes/fbx_viewer_scene.h"
#include "uinta/scenes/terrain_scene.h"
#include "uinta/scenes/trees_scene.h"
#include "uinta/shaders/basic_shader.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  explicit DemoScene(Engine* engine,
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(engine, SCENE_NAME, layer) {
    auto palette = Palettes::PrimaryPalette();
    engine->gl()->clearColor(palette[0].r, palette[0].g, palette[0].b,
                             palette[0].a);

    addSystem<CameraManager>(this);
    addSystem<BasicShaderManager>(this);

    addComponent<CursorManager>(this);

    addScene<DebugScene>();
    addScene<FbxViewerScene>();
    addScene<TerrainScene>();
    addScene<TreeScene>();
  }
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_DEMO_SCENE_H_
