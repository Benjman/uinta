#include <gtest/gtest.h>

#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/logging.hpp>

#define private public
#define protected public
#include <uinta/runner/runner.hpp>

#include "./mock/mock_runner.hpp"

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
  ASSERT_DEATH({ MockRunner runner(getUniqueTestName(), nullptr); }, "File manager must be initialized.")
      << "Application was expected to die.";
}

TEST(RunnerTest, gpu_utils_null) {
  ASSERT_DEATH({ MockRunner runner(getUniqueTestName(), std::make_unique<MockFileManager>(), nullptr); },
               "GPU Utilities must be initialized.")
      << "Application was expected to die.";
}

TEST(RunnerTest, doInit_errorStopsRunner) {
  MockRunner runner;
  const auto error = make_error(error::ExpectedError);
  runner.on_doInit = [error] { return error; };
  EXPECT_THROW(
      {
        try {
          runner.run();
        } catch (const UintaException& ex) {
          ASSERT_STREQ(ex.what(), UintaException::format_message(error.message(), make_error(error::ExpectedError)).c_str())
              << "Unexpected error message.";
          throw ex;
        }
      },
      UintaException)
      << "Unexpected type of exception was thrown.";
}

TEST(RunnerTest, advanceState) {
  f32 runtime = 0;
  f32 runtime_delta = 60.0 / 1000.0;

  MockRunner runner;
  runner.on_runtime = [&] { return runtime += runtime_delta; };

  ASSERT_EQ(0, runner.state().tick) << "Initial `tick` count was expected to be zero.";
  ASSERT_FLOAT_EQ(0, runner.state().delta) << "Initial `delta` count was expected to be zero.";
  ASSERT_FLOAT_EQ(0, runner.state().runtime) << "Initial `runtime` count was expected to be zero.";

  constexpr u32 LoopCount = 1e3;  // limit number of loops because of rounding errors
  for (u32 i = 1; i < LoopCount; ++i) {
    runner.advanceState();
    ASSERT_EQ(i, runner.state().tick) << "Unexpected tick.";
    ASSERT_NEAR(runtime_delta, runner.state().delta, 0.001) << "Unexpect runtime delta.";
    ASSERT_NEAR(runtime, runner.state().runtime, 0.001) << "Unexpected runtime.";
  }
}
