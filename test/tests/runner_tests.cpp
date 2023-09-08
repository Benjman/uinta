#include <gtest/gtest.h>

#include <uinta/logging.hpp>

#define private public
#define protected public
#include <uinta/runner/runner.hpp>

#include "./utils/test_file_manager.hpp"
#include "./utils/test_runner.hpp"

using namespace uinta;

namespace uinta {
enum class error {
  ExpectedError = 100,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::ExpectedError), "This error was produced by unit tests, and was expected."},
};
// NOLINTNEXTLINE
UINTA_ERROR_FRAMEWORK(TestRunner, errorMessages);
}  // namespace uinta

TEST(RunnerTest, file_manager_null) {
  RunnerDependencies dependencies;
  TEST_DEPENDENCIES;
  dependencies.file_manager = nullptr;
  ASSERT_DEATH({ TestRunner runner(RunnerTest_file_manager_null_Test::test_info_->name(), 0, nullptr, std::move(dependencies)); },
               "File manager must be initialized.")
      << "Application was expected to die.";
}

TEST(RunnerTest, gpu_utils_null) {
  RunnerDependencies dependencies;
  TEST_DEPENDENCIES;
  dependencies.gpu_utils = nullptr;
  ASSERT_DEATH({ TestRunner runner(RunnerTest_gpu_utils_null_Test::test_info_->name(), 0, nullptr, std::move(dependencies)); },
               "GPU Utilities must be initialized.")
      << "Application was expected to die.";
}

TEST(RunnerTest, createOpenGLContext_errorStopsRunner) {
  RunnerDependencies dependencies;
  TEST_DEPENDENCIES;
  TestRunner runner(RunnerTest_createOpenGLContext_errorStopsRunner_Test::test_info_->name(), 0, nullptr,
                    std::move(dependencies));
  const auto error = make_error(error::ExpectedError);
  runner.createOpenGLContext_ec = error;
  EXPECT_THROW(
      {
        try {
          runner.run();
        } catch (const UintaException& ex) {
          ASSERT_STREQ(ex.what(), UintaException::format_message(error.message(), make_error(error::ExpectedError)).c_str())
              << "Unexpected error message.";
          ASSERT_TRUE(runner.createOpenGLContext_called) << "`createOpenGLContext()` was not called.";
          throw ex;
        }
      },
      UintaException)
      << "Unexpected type of exception was thrown.";
}

TEST(RunnerTest, doInit_errorStopsRunner) {
  RunnerDependencies dependencies;
  TEST_DEPENDENCIES;
  TestRunner runner(RunnerTest_doInit_errorStopsRunner_Test::test_info_->name(), 0, nullptr, std::move(dependencies));
  const auto error = make_error(error::ExpectedError);
  runner.doInit_ec = error;
  EXPECT_THROW(
      {
        try {
          runner.run();
        } catch (const UintaException& ex) {
          ASSERT_STREQ(ex.what(), UintaException::format_message(error.message(), make_error(error::ExpectedError)).c_str())
              << "Unexpected error message.";
          ASSERT_TRUE(runner.doInit_called) << "`doInit()` was not called.";
          throw ex;
        }
      },
      UintaException)
      << "Unexpected type of exception was thrown.";
}

TEST(RunnerTest, advanceState) {
  RunnerDependencies dependencies;
  TEST_DEPENDENCIES;
  TestRunner runner(RunnerTest_advanceState_Test::test_info_->name(), 0, nullptr, std::move(dependencies));

  ASSERT_EQ(0, runner.state().tick) << "Initial `tick` count was expected to be zero.";
  ASSERT_FLOAT_EQ(0, runner.state().delta) << "Initial `delta` count was expected to be zero.";
  ASSERT_FLOAT_EQ(0, runner.state().runtime) << "Initial `runtime` count was expected to be zero.";

  constexpr u32 LoopCount = 1e3;  // limit number of loops because of rounding errors
  for (u32 i = 1; i < LoopCount; ++i) {
    runner.advanceState();
    ASSERT_EQ(i, runner.state().tick) << "Unexpected tick.";
    ASSERT_NEAR(runner.rt_delta, runner.state().delta, 0.001) << "Unexpect runtime delta.";
    ASSERT_NEAR(runner.rt_delta * runner.state().tick, runner.state().runtime, 0.001) << "Unexpected runtime.";
  }
}
