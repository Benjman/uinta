#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <uinta/glfw_utils/glfw_runner_ui.hpp>
#include <uinta/runner.hpp>

namespace uinta {

class GlfwRunner : public Runner {
  friend void registerCallbacks(GlfwRunner*);
  friend bool createGLFWWindow(GlfwRunner*);
  friend GlfwRunnerUi;

 public:
  GLFWwindow* window = nullptr;
  GlfwRunnerUi ui;

  explicit GlfwRunner(const std::string& title, int argc = 0, const char** argv = nullptr) noexcept : Runner(title, argc, argv) {
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
