#ifndef UINTA_TEST_RUNNER_HPP
#define UINTA_TEST_RUNNER_HPP

#include <uinta/runner/runner.hpp>

namespace uinta {

#define TEST_RUNNER_DEPENDENCIES                                   \
  dependencies.file_manager = std::make_unique<TestFileManager>(); \
  dependencies.gpu_utils = std::make_unique<TestRunnerGpuUtils>();

class TestRunner : public Runner {
 public:
  TestRunner(const std::string& title = "", i32 argc = 0, const char** argv = nullptr, RunnerDependencies dependencies = {})
      : Runner(title, argc, argv, std::move(dependencies)) {
    setFlag(Runner::RENDERING_ENABLED, false, m_flags);
  }

  bool createOpenGLContext_called = false;
  uinta_error_code createOpenGLContext_ec = SUCCESS_EC;
  std::function<void()> m_on_createOpenGLContext;
  virtual uinta_error_code createOpenGLContext() override {
    if (m_on_createOpenGLContext) m_on_createOpenGLContext();
    createOpenGLContext_called = true;
    return createOpenGLContext_ec;
  }

  f32 rt;
  static constexpr f32 rt_delta = 0.0167;
  std::function<void()> m_on_runtime = {};
  virtual f64 runtime() override {
    if (m_on_runtime) m_on_runtime();
    return rt += rt_delta;
  }

  bool pollInput_called = false;
  std::function<void()> m_on_pollInput;
  virtual void pollInput() override {
    if (m_on_pollInput) m_on_pollInput();
    pollInput_called = true;
  }

  bool swapBuffers_called = false;
  std::function<void()> m_on_swapBuffers;
  virtual void swapBuffers() override {
    if (m_on_swapBuffers) m_on_swapBuffers();
    swapBuffers_called = true;
  }

  bool doInit_called = false;
  uinta_error_code doInit_ec = SUCCESS_EC;
  std::function<void()> m_on_doInit;
  virtual uinta_error_code doInit() override {
    if (m_on_doInit) m_on_doInit();
    doInit_called = true;
    return doInit_ec;
  }
};

class TestRunnerGpuUtils : public RunnerGpuUtils {
 public:
  bool init_called = false;
  uinta_error_code ec = SUCCESS_EC;
  uinta_error_code init() {
    init_called = true;
    return ec;
  }

  bool clear_buffer_called = false;
  void clear_buffer(const glm::vec3& color, GLbitfield mask) {
    clear_buffer_called = true;
  }
};

}  // namespace uinta

#endif  // UINTA_TEST_RUNNER_HPP
