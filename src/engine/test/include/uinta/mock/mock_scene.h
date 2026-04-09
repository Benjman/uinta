#ifndef SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_SCENE_H_
#define SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_SCENE_H_

#include "uinta/scene/scene.h"

namespace uinta {

struct MockScene : Scene {
  static size_t TestSceneId;
  explicit MockScene(Engine* engine,
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(engine, layer) {}

  explicit MockScene(Scene* parent,
                     SceneLayer layer = SceneLayer::Simulation) noexcept
      : Scene(parent, layer) {}

  std::function<void(time_t)> onPreTick = [](f32) {};
  void preTick(time_t delta) noexcept override { onPreTick(delta); }

  std::function<void(time_t)> onTick = [](time_t) {};
  void tick(time_t delta) noexcept override { onTick(delta); }

  std::function<void(time_t)> onPostTick = [](time_t) {};
  void postTick(time_t delta) noexcept override { onPostTick(delta); }

  std::function<void(time_t)> onPreRender = [](time_t) {};
  void preRender(time_t delta) noexcept override { onPreRender(delta); }

  std::function<void(time_t)> onRender = [](time_t) {};
  void render(time_t delta) noexcept override { onRender(delta); }

  std::function<void(time_t)> onPostRender = [](time_t) {};
  void postRender(time_t delta) noexcept override { onPostRender(delta); }

  void onDebugUi() noexcept override {}
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_INCLUDE_UINTA_MOCK_MOCK_SCENE_H_
