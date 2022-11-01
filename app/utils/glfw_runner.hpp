#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <string>
#include <uinta/runner.hpp>

#include "./glfw_runner_ui.hpp"

namespace uinta {

class GlfwRunner : public Runner {
  friend void registerCallbacks(GlfwRunner*);
  friend bool createGLFWWindow(GlfwRunner*);
  friend GlfwRunnerUi;

 public:
  GLFWwindow* window = nullptr;
  GlfwRunnerUi ui;

  GlfwRunner(const std::string& title, unsigned int width, unsigned int height) noexcept : Runner(title, width, height) {
  }

  ~GlfwRunner();

 protected:
  virtual bool doInit() override;
  virtual void doPreTick(const RunnerState& state) override;
  virtual void doTick(const RunnerState& state) override;
  virtual void doPostTick(const RunnerState& state) override;
  virtual void doPreRender(const RunnerState& state) override;
  virtual void doRender(const RunnerState& state) override;
  virtual void doPostRender(const RunnerState& state) override;
  virtual void doShutdown() override;

  void swapBuffers() override;
  bool createOpenGLContext() override;
  bool shouldExit() override;
  double getRuntime() const override;
  void pollInput() override;
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_HPP
