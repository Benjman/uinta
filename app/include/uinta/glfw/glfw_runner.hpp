#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <uinta/runner.hpp>

namespace uinta {

class GlfwRunner : public Runner {
  static constexpr i32 MAX_MONITORS = 4;

 public:
  explicit GlfwRunner(const std::string& title, i32 argc = 0, const char** argv = nullptr) noexcept
      : Runner(title, argc, argv, std::make_unique<FileManager_Desktop>(), std::make_unique<RunnerGpuUtils_OpenGL>()) {
  }

  ~GlfwRunner();

  GLFWwindow* glfwWindow() const noexcept {
    return m_window;
  }

  void glfwWindow(GLFWwindow* const v) {
    m_window = v;
  }

  const std::array<Monitor<GLFWmonitor*>, MAX_MONITORS> monitors() const noexcept {
    return m_monitors;
  }

  void monitors(const std::array<Monitor<GLFWmonitor*>, MAX_MONITORS> v) {
    m_monitors = v;
  }

 protected:
  virtual uinta_error_code doInit() override;
  virtual void doPreTick() override;
  virtual void doTick() override;
  virtual void doPostTick() override;
  virtual void doPreRender() override;
  virtual void doRender() override;
  virtual void doPostRender() override;
  virtual void doShutdown() override;

  void swapBuffers() override;
  uinta_error_code createOpenGLContext() override;
  f64 runtime() override;
  void pollInput() override;
  void registerCallbacks();
  void setWindowPosition(u32 x, u32 y);

 private:
  GLFWwindow* m_window = nullptr;
  std::array<Monitor<GLFWmonitor*>, MAX_MONITORS> m_monitors;
};

}  // namespace uinta

#endif  // UINTA_GLFW_RUNNER_HPP
