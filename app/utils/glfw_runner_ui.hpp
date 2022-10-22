#ifndef UINTA_GLFW_RUNNER_UI_HPP
#define UINTA_GLFW_RUNNER_UI_HPP

#include <uinta/flags.hpp>
#include <uinta/gl/api.hpp>

namespace uinta {

class Camera;
class TargetCamera;
class GlfwRunner;

struct GlfwRunnerUiResult {
  static const flag_t SHOW_GRID_FLAG = 0x1;
  static const flag_t INPUT_HANDLED = 0x2;

  flags_t flags = 0;
  float camAngle;
  float camDist;

  GlfwRunnerUiResult() = default;
  GlfwRunnerUiResult(const GlfwRunnerUiResult& other) {
    *this = other;
  }

  GlfwRunnerUiResult& operator=(const GlfwRunnerUiResult& rhs) {
    camAngle = rhs.camAngle;
    camDist = rhs.camDist;
    flags = rhs.flags;
    return *this;
  }
};

class GlfwRunnerUi {
 public:
  float renderTime_micros;
  float tickTime_micros;
  bool cb_showGrid = true;

  GlfwRunnerUi() = default;

  GlfwRunnerUiResult updateAndRender(const GlfwRunner& runner);

  void onInit(const GlfwRunner& runner);
  void onPreTick(const GlfwRunner& runner);
  void onTick(const GlfwRunner& runner);
  void onPostTick(const GlfwRunner& runner);
  void onPreRender(const GlfwRunner& runner);
  void onRender(const GlfwRunner& runner);
  void onPostRender(const GlfwRunner& runner);
  void onShutdown(const GlfwRunner& runner);
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_UI_HPP
