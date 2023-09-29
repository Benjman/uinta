#ifndef UINTA_TEST_RUNNER_HPP
#define UINTA_TEST_RUNNER_HPP

#include <gtest/gtest.h>

#include <uinta/error.hpp>
#include <uinta/runner/runner.hpp>

#include "./mock_file_manager.hpp"

inline std::string getUniqueTestName() {
  const auto* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
  return std::string(test_info->test_suite_name()) + "." + test_info->name();
}

namespace uinta {

class MockRunnerGpuUtils : public RunnerGpuUtils {
 public:
  std::function<uinta_error_code()> m_on_init;
  uinta_error_code init(Runner& runner) {
    if (m_on_init) return m_on_init();
    return SUCCESS_EC;
  }

  std::function<void()> m_on_clear_buffer;
  void clear_buffer(const glm::vec3& color, GLbitfield mask) {
    if (m_on_clear_buffer) m_on_clear_buffer();
  }
};

class MockRunner : public Runner {
 public:
  MockRunner(const std::string& title = getUniqueTestName(),
             std::unique_ptr<FileManager> file_manager = std::make_unique<MockFileManager>(),
             std::unique_ptr<RunnerGpuUtils> gpu_utils = std::make_unique<MockRunnerGpuUtils>())
      : Runner(title, 0, nullptr, std::move(file_manager), std::move(gpu_utils)) {
  }

  std::function<uinta_error_code()> on_createOpenGLContext;
  virtual uinta_error_code init_gpu_context() override {
    if (on_createOpenGLContext) return on_createOpenGLContext();
    return SUCCESS_EC;
  }

  std::function<f32()> on_runtime;
  virtual f64 runtime() override {
    if (on_runtime) return on_runtime();
    return 0;
  }

  std::function<void()> on_pollInput;
  virtual void pollInput() override {
    if (on_pollInput) on_pollInput();
  }

  std::function<void()> on_swapBuffers;
  virtual void swapBuffers() override {
    if (on_swapBuffers) on_swapBuffers();
  }

  std::function<uinta_error_code()> on_doInit;
  virtual uinta_error_code doInit() override {
    if (on_doInit) return on_doInit();
    return SUCCESS_EC;
  }
};

}  // namespace uinta

#endif  // UINTA_TEST_RUNNER_HPP
