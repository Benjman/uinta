#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <uinta/runner.hpp>

namespace uinta {

class GlfwRunner : public Runner {
 public:
  GLFWwindow* window = nullptr;

  explicit GlfwRunner(const std::string& title, i32 argc = 0, const char** argv = nullptr) noexcept : Runner(title, argc, argv) {
  }

  ~GlfwRunner();

 protected:
  virtual uinta_error_code doInit() override;
  virtual void doPreTick(const RunnerState& state) override;
  virtual void doTick(const RunnerState& state) override;
  virtual void doPostTick(const RunnerState& state) override;
  virtual void doPreRender(const RunnerState& state) override;
  virtual void doRender(const RunnerState& state) override;
  virtual void doPostRender(const RunnerState& state) override;
  virtual void doShutdown() override;

  void swapBuffers() override;
  uinta_error_code createOpenGLContext() override;
  f64 getRuntime() const override;
  void pollInput() override;
  void registerCallbacks();
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_HPP
