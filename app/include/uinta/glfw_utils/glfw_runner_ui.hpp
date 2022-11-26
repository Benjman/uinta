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
  void onPreTick(GlfwRunner& runner);
  void onTick(GlfwRunner& runner);
  void onPostTick(GlfwRunner& runner);
  void onPreRender(GlfwRunner& runner);
  void onRender(GlfwRunner& runner);
  void onPostRender(GlfwRunner& runner);
  void onShutdown(GlfwRunner& runner);
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_UI_HPP
