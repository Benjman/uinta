#ifndef UINTA_GLFW_RUNNER_HPP
#define UINTA_GLFW_RUNNER_HPP

#include <array>
#include <uinta/runner.hpp>
#include <uinta/runner/monitor.hpp>

namespace uinta {

class GlfwRunner : public Runner {
  static constexpr i32 MAX_MONITORS = 4;

 public:
  explicit GlfwRunner(const std::string& title, i32 argc = 0, const char** argv = nullptr);

  ~GlfwRunner() override;

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
  void swapBuffers() override;
  uinta_error_code init_gpu_context() override;
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
