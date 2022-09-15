#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <uinta/runner.hpp>

namespace uinta {

class Camera;

class GlfwRunner : public Runner {
 public:
  GLFWwindow* window;

  GlfwRunner(const std::string& title, unsigned int width, unsigned int height) noexcept : Runner(title, width, height) {}
  ~GlfwRunner();

 protected:
  virtual bool doInit() override { return true; }

  virtual void doPreTick(const RunnerState&) override {}
  virtual void doTick(const RunnerState&) override {}
  virtual void doPostTick(const RunnerState&) override {}

  virtual void doPreRender(const RunnerState& state) override {}
  virtual void doRender(const RunnerState& state) override {}
  virtual void doPostRender(const RunnerState& state) override {}

  virtual void doShutdown() override {}

  virtual void doHandleWindowSizeChanged(const int width, const int height) override {}

  bool internalInit() override;

  void swapBuffers() override;
  void internalPreRender() override;
  void internalRender() override;
  void internalPostRender() override;

  void internalShutdown() override;

  bool shouldExit() override;

  double getRuntime() override;
  void pollInput() override;
  void register_callbacks();
};

void createGLFWWindow(GlfwRunner&);

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_HPP
