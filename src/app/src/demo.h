#ifndef SRC_APP_SRC_DEMO_H_
#define SRC_APP_SRC_DEMO_H_

#include "uinta/debug/debug_scene.h"
#include "uinta/engine.h"
#include "uinta/scene.h"

namespace uinta {

class DemoScene : public DebugScene {
 public:
  explicit DemoScene(Engine* engine) noexcept
      : DebugScene({engine, OpenGLApiImpl::Instance()}, Layer::Simulation) {}
};

}  // namespace uinta

#endif  // SRC_APP_SRC_DEMO_H_
