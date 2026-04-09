#ifndef SRC_ENGINE_TEST_SRC_UTILS_H_
#define SRC_ENGINE_TEST_SRC_UTILS_H_

#include <gtest/gtest.h>

#include "uinta/engine/engine.h"
#include "uinta/engine/service_registry.h"
#include "uinta/mock/mock_app_config.h"
#include "uinta/mock/mock_gl.h"

namespace uinta {

class UintaTestF : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  ServiceRegistry serviceRegistry_;
  ArgsProcessor args = ArgsProcessor(0, nullptr);
  MockAppConfig appConfig = MockAppConfig(&args);

  Engine makeEngine(Platform* platform) noexcept {
    serviceRegistry_.registerService<const OpenGLApi>(&gl);
    serviceRegistry_.registerService<const ArgsProcessor>(&args);
    serviceRegistry_.registerService<AppConfig>(&appConfig);
    return Engine({
        .serviceRegistry = &serviceRegistry_,
        .platform = platform,
        .appConfig = &appConfig,
    });
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_UTILS_H_
