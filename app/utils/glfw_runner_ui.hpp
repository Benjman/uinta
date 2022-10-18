#ifndef UINTA_GLFW_RUNNER_UI_HPP
#define UINTA_GLFW_RUNNER_UI_HPP

#include <uinta/flags.hpp>

namespace uinta {

class Camera;
class TargetCamera;
class GlfwRunner;

static const flag_t GLFW_RUNNER_UI_INPUT_HANDLED = 1 << 0;

class GlfwRunnerUi {
 public:
  float renderTime_micros;
  float tickTime_micros;
  flags_t flags;

  flags_t updateAndRender(GlfwRunner& runner);

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