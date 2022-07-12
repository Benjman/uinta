#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <GLFW/glfw3.h>

#include <uinta/runner.hpp>

namespace uinta {

class Camera;

struct GlfwRunner : Runner {
  GLFWwindow* window;

  GlfwRunner(const std::string& title, unsigned int width, unsigned int height) noexcept : Runner(title, width, height) {}
  ~GlfwRunner();

 protected:
  virtual bool doInit() override { return true; }
  virtual void doInitResources() override {}

  virtual void doPreTick(const RunnerState&) override {}
  virtual void doTick(const RunnerState&) override {}
  virtual void doPostTick(const RunnerState&) override {}

  virtual void doPreRender() override {}
  virtual void doRender() override {}
  virtual void doPostRender() override {}

  virtual void doShutdown() override {}

  bool internalInit() override;

  void internalPreTick() override;
  void internalTick() override;
  void internalPostTick() override;

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
