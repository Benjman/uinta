#include <gtest/gtest.h>

#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/logging.hpp>

#define private public
#define protected public
#include <uinta/runner/runner.hpp>

#include "./mock/mock_runner.hpp"
#include "./mock/mock_scene.hpp"

using namespace uinta;

namespace uinta {
enum class error {
  ExpectedError = 100,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::ExpectedError), "This error was produced by unit tests, and was expected."},
};
// NOLINTNEXTLINE
UINTA_ERROR_FRAMEWORK(TestRunner, errorMessages);
}  // namespace uinta

TEST(Runner, file_manager_null) {
  ASSERT_DEATH({ MockRunner runner(getUniqueTestName(), nullptr); }, "File manager must be initialized.")
      << "Application was expected to die.";
}

TEST(Runner, gpu_utils_null) {
  ASSERT_DEATH({ MockRunner runner(getUniqueTestName(), std::make_unique<MockFileManager>(), nullptr); },
               "GPU Utilities must be initialized.")
      << "Application was expected to die.";
}

TEST(Runner, advanceState) {
  f32 runtime = 0;
  f32 runtime_delta = 60.0 / 1000.0;

  MockRunner runner;
  runner.on_runtime = [&] { return runtime += runtime_delta; };

  ASSERT_EQ(0, runner.state().tick) << "Initial `tick` count was expected to be zero.";
  ASSERT_FLOAT_EQ(0, runner.state().delta) << "Initial `delta` count was expected to be zero.";
  ASSERT_FLOAT_EQ(0, runner.state().runtime) << "Initial `runtime` count was expected to be zero.";

  constexpr u32 LoopCount = 1e5;  // limit number of loops because of rounding errors
  for (u32 i = 1; i < LoopCount; ++i) {
    runner.advanceState();
    ASSERT_EQ(i, runner.state().tick) << "Unexpected tick.";
    ASSERT_NEAR(runtime_delta, runner.state().delta, 0.001) << "Unexpect runtime delta.";
    ASSERT_NEAR(runtime, runner.state().runtime, 0.001) << "Unexpected runtime.";
  }
}

TEST(Runner, scenes__null_value) {
  ASSERT_EQ("Invalid scene.", MockRunner().add_scene(nullptr).message());
}

TEST(Runner, scenes__initial_state) {
  ASSERT_TRUE(MockRunner().m_scenes.empty()) << "Scene stack was expected to be empty.";
}

TEST(Runner, scenes__add_in_created_state) {
  MockRunner runner;
  auto scene = std::make_unique<MockScene>(runner);
  ASSERT_FALSE(runner.add_scene(std::move(scene))) << "Adding scene expected to return a success code.";
  ASSERT_EQ(1, runner.m_scenes.size()) << "Expected to have a single value in the scene stack.";
  ASSERT_EQ(Scene::State::Created, runner.m_scenes.front()->state()) << "Unexpected state after adding a scene to the stack.";
}

TEST(Runner, scenes__fill) {
  MockRunner runner;
  std::list<std::unique_ptr<Scene>>::iterator itr;

  auto check_order = [&runner](const std::vector<std::string> &order) {
    size_t index = 0;
    auto itr = runner.m_scenes.begin();
    for (const auto &name : order) {
      ASSERT_EQ(name, itr->get()->name()) << "Unexpected scene: " << name;
      itr++;
      index++;
    }
    ASSERT_EQ(index, runner.m_scenes.size()) << "Unexpected scene stack size.";
  };

  auto debug = std::make_unique<MockScene>(runner, Scene::Layer::Debug, "debug");
  auto debug_name = debug->name();
  ASSERT_FALSE(runner.add_scene(std::move(debug))) << "Adding scene expected to succeed.";
  check_order({debug_name});

  auto ui = std::make_unique<MockScene>(runner, Scene::Layer::UI, "ui");
  auto ui_name = ui->name();
  ASSERT_FALSE(runner.add_scene(std::move(ui))) << "Adding scene expected to succeed.";
  check_order({ui_name, debug_name});

  auto simulation = std::make_unique<MockScene>(runner, Scene::Layer::Simulation, "simulation");
  auto simulation_name = simulation->name();
  ASSERT_FALSE(runner.add_scene(std::move(simulation))) << "Adding scene expected to succeed.";
  check_order({simulation_name, ui_name, debug_name});

  auto second_simulation = std::make_unique<MockScene>(runner, Scene::Layer::Simulation, "second_simulation");
  auto second_simulation_name = second_simulation->name();
  ASSERT_FALSE(runner.add_scene(std::move(second_simulation))) << "Adding scene expected to succeed.";
  check_order({simulation_name, second_simulation_name, ui_name, debug_name});

  auto second_ui = std::make_unique<MockScene>(runner, Scene::Layer::UI, "second_ui");
  auto second_ui_name = second_ui->name();
  ASSERT_FALSE(runner.add_scene(std::move(second_ui))) << "Adding scene expected to succeed.";
  check_order({simulation_name, second_simulation_name, ui_name, second_ui_name, debug_name});

  auto second_debug = std::make_unique<MockScene>(runner, Scene::Layer::Debug, "second_debug");
  auto second_debug_name = second_debug->name();
  ASSERT_FALSE(runner.add_scene(std::move(second_debug))) << "Adding scene expected to succeed.";
  check_order({simulation_name, second_simulation_name, ui_name, second_ui_name, debug_name, second_debug_name});
}

TEST(Runner, find_scene) {
  MockRunner runner;
  ASSERT_EQ(0, runner.scenes().size()) << "Expected scene stack to be empty.";

  class SceneA : public MockScene {
   public:
    SceneA(MockRunner &runner) : MockScene(runner, Layer::Debug, "SceneA") {
    }
  };

  class SceneB : public MockScene {
   public:
    SceneB(MockRunner &runner) : MockScene(runner, Layer::Debug, "SceneB") {
    }
  };

  ASSERT_FALSE(runner.find_scenec<SceneA>()) << "Scene was not expected to be found.";
  ASSERT_FALSE(runner.find_scenec<SceneB>()) << "Scene was not expected to be found.";

  ASSERT_FALSE(runner.add_scene(std::make_unique<SceneA>(runner)));
  ASSERT_TRUE(runner.find_scenec<SceneA>()) << "Scene was expected to be found.";
  ASSERT_FALSE(runner.find_scenec<SceneB>()) << "Scene was not expected to be found.";

  ASSERT_FALSE(runner.add_scene(std::make_unique<SceneB>(runner)));
  ASSERT_TRUE(runner.find_scenec<SceneA>()) << "Scene was expected to be found.";
  ASSERT_TRUE(runner.find_scenec<SceneB>()) << "Scene was expected to be found.";

  ASSERT_FALSE(runner.remove_scene(runner.find_scenec<SceneB>()));
  ASSERT_TRUE(runner.find_scenec<SceneA>()) << "Scene was expected to be found.";
  ASSERT_FALSE(runner.find_scenec<SceneB>()) << "Scene was not expected to be found.";

  ASSERT_FALSE(runner.remove_scene(runner.find_scenec<SceneA>()));
  ASSERT_FALSE(runner.find_scenec<SceneA>()) << "Scene was not expected to be found.";
  ASSERT_FALSE(runner.find_scenec<SceneB>()) << "Scene was not expected to be found.";
}
