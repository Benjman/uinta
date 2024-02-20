#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"

namespace uinta {

struct TestCapabilityGuard final : CapabilityGuard {
  TestCapabilityGuard(GLenum capability, bool isActive,
                      const OpenGLApi* gl) noexcept
      : CapabilityGuard(capability, isActive, gl) {}
};

TEST(CapabilityGuard, StateReversion) {
  auto cap = GL_DEPTH_TEST;
  auto invalidCap = GL_NONE;

  MockOpenGLApi gl;

  auto onIsEnabledCalledCap = invalidCap;
  gl.onIsEnabled = [&](auto capability) {
    onIsEnabledCalledCap = capability;
    return false;
  };

  auto onEnableCalledCap = invalidCap;
  gl.onEnable = [&](auto capability) { onEnableCalledCap = capability; };

  auto onDisableCalledCap = invalidCap;
  gl.onDisable = [&](auto capability) { onDisableCalledCap = capability; };

  {
    TestCapabilityGuard guard(cap, true, &gl);
    ASSERT_EQ(cap, onIsEnabledCalledCap) << "glIsEnabled() wasn't called.";
    ASSERT_EQ(cap, onEnableCalledCap) << "glEnable() wasn't called.";
    ASSERT_EQ(invalidCap, onDisableCalledCap)
        << "glDisable() shouldn't have been called until destructor.";
  }

  ASSERT_EQ(cap, onDisableCalledCap) << "glDisable() wasn't called.";
}

}  // namespace uinta
