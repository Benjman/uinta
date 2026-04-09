#ifndef SRC_ENGINE_TEST_SRC_UTILS_H_
#define SRC_ENGINE_TEST_SRC_UTILS_H_

#include <gtest/gtest.h>

#include "uinta/args.h"
#include "uinta/engine/engine.h"
#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_gl.h"

namespace uinta {

class UintaTestF : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  ArgsProcessor args_ = ArgsProcessor(0, nullptr);
  MockAppConfig appConfig_;

  EngineDependencies engineDependencies(Platform* platform) noexcept {
    return {
        .platform = platform,
        .gl = &gl,
        .appConfig = &appConfig_,
        .args = &args_,
    };
  }

  Engine makeEngine(Platform* platform) noexcept { return Engine(engineDependencies(platform)); }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_UTILS_H_
