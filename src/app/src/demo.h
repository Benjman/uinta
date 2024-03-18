#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include "./scenes/fbx_viewer.h"
#include "uinta/scene.h"

namespace uinta {

class DemoScene : public Scene {
 public:
  DemoScene() noexcept : Scene(Layer::Simulation) {
    addScene<FbxViewerScene>("pawn.fbx");
  }
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
