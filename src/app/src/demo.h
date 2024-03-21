#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include <string>

#include "uinta/camera/camera.h"
#include "uinta/camera/camera_manager.h"
#include "uinta/debug/debug_scene.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/cursor_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/scenes/fbx_viewer.h"
#include "uinta/scenes/trees.h"
#include "uinta/shaders/primitive.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  explicit DemoScene(Scene* parent, Engine* engine) noexcept
      : Scene(parent, engine, Layer::Simulation), shader_(engine->gl()) {
    camera_.angle(315);
    camera_.dist(5);
    camera_.pitch(25);
    camera_.force();

    auto* cameraManager = addSystem<CameraManager>(&camera_);
    addComponent<CursorManager>(engine, cameraManager);
    addSystem<PrimitiveShaderManager>(&shader_, engine->dispatchers(),
                                      cameraManager);

    engine->addListener<EngineEvent::SceneAdded>([&](const auto& event) {
      logInfo(absl::StrFormat("Scene added: %s", event.scene->name()).c_str());
    });

    DebugSceneParams params(&camera_, &shader_);
    addScene<DebugScene>(&params);
    addScene<FbxViewerScene>(&params);
    addScene<TreeScene>(&params);
  }

  std::string name() const noexcept override { return "DemoScene"; }

 private:
  PrimitiveShader shader_;
  Camera camera_;
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
