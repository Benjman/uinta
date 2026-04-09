#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"

namespace uinta {
namespace {

class DepthTestGuardTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;

  GLenum isEnabledCalledCap = GL_NONE;
  GLenum enableCalledCap = GL_NONE;
  GLenum disableCalledCap = GL_NONE;
  int enableCallCount = 0;
  int disableCallCount = 0;

  void SetUp() override {
    gl.onIsEnabled = [&](auto capability) {
      isEnabledCalledCap = capability;
      return false;
    };
    gl.onEnable = [&](auto capability) {
      enableCalledCap = capability;
      enableCallCount++;
    };
    gl.onDisable = [&](auto capability) {
      disableCalledCap = capability;
      disableCallCount++;
    };
  }
};

TEST_F(DepthTestGuardTest, UsesDepthTestCapability) {
  {
    DepthTestGuard guard(true, &gl);
    ASSERT_EQ(GL_DEPTH_TEST, isEnabledCalledCap);
    ASSERT_EQ(GL_DEPTH_TEST, enableCalledCap);
  }
  ASSERT_EQ(GL_DEPTH_TEST, disableCalledCap);
}

TEST_F(DepthTestGuardTest, ActiveByDefault) {
  {
    DepthTestGuard guard(true, &gl);
    ASSERT_EQ(1, enableCallCount);
  }
  ASSERT_EQ(1, disableCallCount);
}

TEST_F(DepthTestGuardTest, InactiveDoesNotEnable) {
  {
    DepthTestGuard guard(false, &gl);
    ASSERT_EQ(0, enableCallCount);
  }
  ASSERT_EQ(0, disableCallCount);
}

TEST_F(DepthTestGuardTest, AlreadyEnabledNoChange) {
  gl.onIsEnabled = [&](auto capability) {
    isEnabledCalledCap = capability;
    return true;  // Already enabled
  };

  {
    DepthTestGuard guard(true, &gl);
    ASSERT_EQ(0, enableCallCount);
  }
  ASSERT_EQ(0, disableCallCount);
}

TEST_F(DepthTestGuardTest, RestoresStateOnDestruction) {
  {
    DepthTestGuard guard(true, &gl);
    ASSERT_EQ(GL_NONE, disableCalledCap);
  }
  ASSERT_EQ(GL_DEPTH_TEST, disableCalledCap);
  ASSERT_EQ(1, disableCallCount);
}

}  // namespace
}  // namespace uinta
