#ifndef UINTA_GLFW_RUNNER_UI_HPP
#define UINTA_GLFW_RUNNER_UI_HPP

#include <uinta/fwd.hpp>

namespace uinta {

class GlfwRunner;

static const flag_t INPUT_HANDLED_MOUSE = 1 << 0;
static const flag_t INPUT_HANDLED_KEYBOARD = 1 << 1;

class GlfwRunnerUi {
 public:
  float renderTime_micros;
  float tickTime_micros;
  bool showingWindow = false;
  flags_t flags;

  void onInit(GlfwRunner& runner);
  void onPreTick(GlfwRunner& runner, const RunnerState& state);
  void onTick(GlfwRunner& runner, const RunnerState& state);
  void onPostTick(GlfwRunner& runner, const RunnerState& state);
  void onPreRender(GlfwRunner& runner, const RunnerState& state);
  void onRender(GlfwRunner& runner, const RunnerState& state);
  void onPostRender(GlfwRunner& runner, const RunnerState& state);
  void onShutdown(GlfwRunner& runner);
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_UI_HPP
