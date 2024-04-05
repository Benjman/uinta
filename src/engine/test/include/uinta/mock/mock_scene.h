#ifndef SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_SCENE_H_
#define SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_SCENE_H_

#include <string>

#include "uinta/engine/engine_state.h"
#include "uinta/scene/scene.h"

namespace uinta {

struct MockScene : Scene {
  static size_t TestSceneId;
  explicit MockScene(Engine* engine,
                     std::string name = absl::StrFormat("Scene%i",
                                                        TestSceneId++),
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(engine, name, layer) {}

  explicit MockScene(Scene* parent,
                     std::string name = absl::StrFormat("Scene%i",
                                                        TestSceneId++),
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(parent, name, layer) {}

  std::function<void(time_t)> onPreTick = [](f32) {};
  void preTick(time_t delta) noexcept { onPreTick(delta); }

  std::function<void(time_t)> onTick = [](time_t) {};
  void tick(time_t delta) noexcept { onTick(delta); }

  std::function<void(time_t)> onPostTick = [](time_t) {};
  void postTick(time_t delta) noexcept { onPostTick(delta); }

  std::function<void(time_t)> onPreRender = [](time_t) {};
  void preRender(time_t delta) noexcept { onPreRender(delta); }

  std::function<void(time_t)> onRender = [](time_t) {};
  void render(time_t delta) noexcept { onRender(delta); }

  std::function<void(time_t)> onPostRender = [](time_t) {};
  void postRender(time_t delta) noexcept { onPostRender(delta); }

  virtual void onDebugUi() noexcept {}
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_SCENE_H_
