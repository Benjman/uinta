#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <uinta/runner.hpp>

#include "./glfw_runner_ui.hpp"

namespace uinta {

class Camera;

class GlfwRunner : public Runner {
  friend void registerCallbacks(GlfwRunner*);
  friend bool createGLFWWindow(GlfwRunner*);

 public:
  GLFWwindow* window = nullptr;
  GlfwRunnerUi ui{};

  GlfwRunner(const std::string& title, unsigned int width, unsigned int height) noexcept : Runner(title, width, height) {
  }

  ~GlfwRunner();

  virtual bool doInit() override;
  virtual void doPostRender(RunnerState& state) override;
  virtual void doTick(RunnerState& state) override;
  virtual void doPreTick(RunnerState& state) override;
  virtual void doPostTick(RunnerState& state) override;
  virtual void doPreRender(RunnerState& state) override;
  virtual void doRender(RunnerState& state) override;
  virtual void doShutdown() override;

  virtual void doHandleWindowSizeChanged(const int width, const int height) override;

  void swapBuffers() override;
  bool shouldExit() override;
  double getRuntime() const override;
  void pollInput() override;
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_HPP
