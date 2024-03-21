#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include "./scenes/fbx_viewer.h"
#include "./scenes/trees.h"
#include "uinta/scenes/debug.h"

namespace uinta {

class DemoScene : public DebugScene {
 public:
  DemoScene() noexcept : DebugScene(Layer::Simulation) {
    addScene<FbxViewerScene>("pawn.fbx", &shader_);
    addScene<TreeScene>(&shader_, camera_);
  }
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
