#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <uinta/runner.hpp>

namespace uinta {

class Camera;

class GlfwRunner : public Runner {
  friend void registerCallbacks(GlfwRunner*);
  friend bool createGLFWWindow(GlfwRunner*);

 public:
  GlfwRunner(const std::string& title, unsigned int width, unsigned int height) noexcept : Runner(title, width, height) {
  }

  ~GlfwRunner();

 protected:
  GLFWwindow* window;

  virtual bool doInit() override;
  virtual void doHandleWindowSizeChanged(const int width, const int height) override;
  virtual void doPostRender(const RunnerState& state) override;
  virtual void doPostTick(const RunnerState& state) override;
  virtual void doPreRender(const RunnerState& state) override;
  virtual void doPreTick(const RunnerState& state) override;
  virtual void doRender(const RunnerState& state) override;
  virtual void doShutdown() override;
  virtual void doTick(const RunnerState& state) override;

  void swapBuffers() override;
  bool shouldExit() override;
  double getRuntime() override;
  void pollInput() override;
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_HPP
