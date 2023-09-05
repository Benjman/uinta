#ifndef UINTA_TEST_RUNNER_HPP
#define UINTA_TEST_RUNNER_HPP

#include <cmath>
#include <uinta/runner/runner.hpp>

namespace uinta {

enum class error {
  ExpectedError = 100,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::ExpectedError), "This error was produced by unit tests, and was expected."},
};
// NOLINTNEXTLINE
UINTA_ERROR_FRAMEWORK(TestRunner, errorMessages);

class TestRunner : public Runner {
 public:
  TestRunner(const std::string& title = "", i32 argc = 0, const char** argv = nullptr, RunnerDependencies dependencies = {})
      : Runner(title, argc, argv, std::move(dependencies)) {
  }

  bool createOpenGLContext_called = false;
  uinta_error_code createOpenGLContext_ec = SUCCESS_EC;
  uinta_error_code createOpenGLContext() override {
    createOpenGLContext_called = true;
    return createOpenGLContext_ec;
  }

  u32 runtime_count = 0;
  f32 runtime_delta = 1.5;
  f32 runtime_exponent = 1.1;
  f64 runtime() override {
    // Exponent to add some variety to the resulting delta
    return runtime_delta * pow(++runtime_count, runtime_exponent);
  }

  bool pollInput_called = false;
  void pollInput() override {
    pollInput_called = true;
  }

  bool swapBuffers_called = false;
  void swapBuffers() override {
    swapBuffers_called = true;
  }

  bool doInit_called = false;
  uinta_error_code doInit_ec = SUCCESS_EC;
  uinta_error_code doInit() override {
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
