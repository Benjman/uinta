#ifndef SRC_ENGINE_TEST_SRC_UTILS_H_
#define SRC_ENGINE_TEST_SRC_UTILS_H_

#include <gtest/gtest.h>

#include "uinta/engine/engine.h"
#include "uinta/localization/locale.h"
#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_gl.h"

namespace uinta {

class UintaTestF : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  ArgsProcessor args = ArgsProcessor(0, nullptr);
  MockAppConfig appConfig = MockAppConfig(&args);

  Engine makeEngine(Platform* platform) noexcept {
    return Engine({
        .platform = platform,
        .appConfig = &appConfig,
        .args = &args,
        .gl = &gl,
        .locale = Locale::EnUs,
    });
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_UTILS_H_
