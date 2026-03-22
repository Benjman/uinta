#ifndef SRC_ENGINE_TEST_SRC_UTILS_H_
#define SRC_ENGINE_TEST_SRC_UTILS_H_

#include <gtest/gtest.h>

#include "uinta/engine/engine.h"
#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_gl.h"

namespace uinta {

inline void setupDefaultCameraConfig(MockAppConfig* appConfig) noexcept {
  appConfig->onGetFloat = [](const std::string&) -> std::optional<f32> {
    return 1.0f;
  };
  appConfig->onGetInt = [](const std::string&) -> std::optional<i32> {
    return 0;
  };
  appConfig->onGetString =
      [](const std::string& key) -> std::optional<std::string> {
    if (key.find("keyboard") != std::string::npos) {
      return "W";
    }
    if (key.find("mouse") != std::string::npos) {
      return "LEFT";
    }
    return std::nullopt;
  };
}

class UintaTestF : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  ArgsProcessor args = ArgsProcessor(0, nullptr);
  MockAppConfig appConfig = MockAppConfig(&args);

  void SetUp() override { setupDefaultCameraConfig(&appConfig); }

  Engine makeEngine(Platform* platform) noexcept {
    return Engine({
        .platform = platform,
        .appConfig = &appConfig,
        .args = &args,
        .gl = &gl,
    });
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_UTILS_H_
