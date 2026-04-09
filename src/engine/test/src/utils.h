#ifndef SRC_ENGINE_TEST_SRC_UTILS_H_
#define SRC_ENGINE_TEST_SRC_UTILS_H_

#include <gtest/gtest.h>

#include "uinta/engine/engine.h"
#include "uinta/engine/service_registry.h"
#include "uinta/gl.h"
#include "uinta/mock/mock_gl.h"

namespace uinta {

class UintaTestF : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  ServiceRegistry serviceRegistry_;

  Engine makeEngine(Platform* platform) noexcept {
    serviceRegistry_.registerService<const OpenGLApi>(&gl);
    return Engine({
        .serviceRegistry = &serviceRegistry_,
        .platform = platform,
    });
  }
};

}  // namespace uinta

#endif  // SRC_ENGINE_TEST_SRC_UTILS_H_
