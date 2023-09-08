#ifndef UINTA_TEST_RUNNER_HPP
#define UINTA_TEST_RUNNER_HPP

#include <cmath>
#include <uinta/runner/runner.hpp>

#include "./test_file_manager.hpp"

namespace uinta {

#define TEST_DEPENDENCIES                                          \
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
  virtual uinta_error_code createOpenGLContext() override {
    createOpenGLContext_called = true;
    return createOpenGLContext_ec;
  }

  f32 rt;
  static constexpr f32 rt_delta = 0.0167;
  virtual f64 runtime() override {
    return rt += rt_delta;
  }

  bool pollInput_called = false;
  virtual void pollInput() override {
    pollInput_called = true;
  }

  bool swapBuffers_called = false;
  virtual void swapBuffers() override {
    swapBuffers_called = true;
  }

  bool doInit_called = false;
  uinta_error_code doInit_ec = SUCCESS_EC;
  virtual uinta_error_code doInit() override {
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
