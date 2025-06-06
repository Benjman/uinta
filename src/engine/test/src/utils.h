#ifndef SRC_ENGINE_TEST_SRC_UTILS_H_
#define SRC_ENGINE_TEST_SRC_UTILS_H_

#include <gtest/gtest.h>

#include "uinta/engine/engine.h"
#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_file_system.h"
#include "uinta/mock/mock_gl.h"

namespace uinta {

class UintaTestF : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  MockFileSystem fileSystem;
  ArgsProcessor args = ArgsProcessor(0, nullptr);
  MockAppConfig appConfig = MockAppConfig(&args, &fileSystem);

  Engine makeEngine(Platform* platform) noexcept {
    return Engine({platform, &appConfig, &gl, &fileSystem});
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_UTILS_H_
