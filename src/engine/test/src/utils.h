#ifndef SRC_ENGINE_TEST_SRC_UTILS_H_
#define SRC_ENGINE_TEST_SRC_UTILS_H_

#include <gtest/gtest.h>

#include <optional>

#include "uinta/args.h"
#include "uinta/engine/engine.h"
#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_gl.h"
#include "uinta/viewport/viewport_manager.h"

namespace uinta {

class UintaTestF : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  ArgsProcessor args_ = ArgsProcessor(0, nullptr);
  MockAppConfig appConfig_;
  std::optional<ViewportManager> viewport_;

  EngineDependencies engineDependencies(Platform* platform) noexcept {
    // Constructed lazily so it reads appConfig_ after SetUp() has configured it.
    if (!viewport_) {
      viewport_.emplace(&appConfig_);
    }
    return {
        .platform = platform,
        .gl = &gl,
        .appConfig = &appConfig_,
        .args = &args_,
        .viewport = &*viewport_,
    };
  }

  Engine makeEngine(Platform* platform) noexcept { return Engine(engineDependencies(platform)); }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_UTILS_H_
