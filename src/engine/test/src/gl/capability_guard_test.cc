#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"

namespace uinta {
namespace {

struct TestCapabilityGuard final : CapabilityGuard {
  TestCapabilityGuard(GLenum capability, bool isActive,
                      const OpenGLApi* gl) noexcept
      : CapabilityGuard(capability, isActive, gl) {}

  using CapabilityGuard::activate;
};

class CapabilityGuardTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
  GLenum cap = GL_DEPTH_TEST;
  GLenum invalidCap = GL_NONE;

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

TEST_F(CapabilityGuardTest, StateReversionWhenDisabled) {
  {
    TestCapabilityGuard guard(cap, true, &gl);
    ASSERT_EQ(cap, isEnabledCalledCap) << "glIsEnabled() wasn't called.";
    ASSERT_EQ(cap, enableCalledCap) << "glEnable() wasn't called.";
    ASSERT_EQ(invalidCap, disableCalledCap)
        << "glDisable() shouldn't have been called until destructor.";
  }
  ASSERT_EQ(cap, disableCalledCap) << "glDisable() wasn't called.";
}

TEST_F(CapabilityGuardTest, WasEnabledNoChange) {
  gl.onIsEnabled = [&](auto capability) {
    isEnabledCalledCap = capability;
    return true;  // Capability was already enabled
  };

  {
    TestCapabilityGuard guard(cap, true, &gl);
    ASSERT_EQ(cap, isEnabledCalledCap) << "glIsEnabled() wasn't called.";
    ASSERT_EQ(invalidCap, enableCalledCap)
        << "glEnable() shouldn't be called when already enabled.";
  }
  ASSERT_EQ(invalidCap, disableCalledCap)
      << "glDisable() shouldn't be called when capability was already enabled.";
}

TEST_F(CapabilityGuardTest, InactiveGuardNoEnable) {
  {
    TestCapabilityGuard guard(cap, false, &gl);
    ASSERT_EQ(cap, isEnabledCalledCap) << "glIsEnabled() wasn't called.";
    ASSERT_EQ(invalidCap, enableCalledCap)
        << "glEnable() shouldn't be called when isActive=false.";
  }
}

TEST_F(CapabilityGuardTest, InactiveGuardNoDisableOnDestruct) {
  {
    TestCapabilityGuard guard(cap, false, &gl);
  }
  ASSERT_EQ(invalidCap, disableCalledCap)
      << "glDisable() shouldn't be called for inactive guard.";
}

TEST_F(CapabilityGuardTest, ManualActivate) {
  TestCapabilityGuard guard(cap, false, &gl);
  ASSERT_EQ(invalidCap, enableCalledCap)
      << "glEnable() shouldn't be called before activate().";

  guard.activate();
  ASSERT_EQ(cap, enableCalledCap)
      << "glEnable() wasn't called after activate().";
  ASSERT_EQ(1, enableCallCount) << "glEnable() should be called exactly once.";
}

TEST_F(CapabilityGuardTest, ManualActivateTwiceOnlyEnablesOnce) {
  TestCapabilityGuard guard(cap, false, &gl);

  guard.activate();
  guard.activate();

  ASSERT_EQ(1, enableCallCount) << "glEnable() should only be called once even "
                                   "with multiple activate() calls.";
}

TEST_F(CapabilityGuardTest, ManualActivateDisablesOnDestruct) {
  {
    TestCapabilityGuard guard(cap, false, &gl);
    guard.activate();
    ASSERT_EQ(invalidCap, disableCalledCap);
  }
  ASSERT_EQ(cap, disableCalledCap)
      << "glDisable() should be called on destruction after manual activate().";
}

TEST_F(CapabilityGuardTest, ActiveGuardAlreadyEnabledNoRedundantCalls) {
  gl.onIsEnabled = [&](auto capability) {
    isEnabledCalledCap = capability;
    return true;  // Already enabled
  };

  {
    TestCapabilityGuard guard(cap, true, &gl);
    ASSERT_EQ(0, enableCallCount) << "glEnable() shouldn't be called.";
  }
  ASSERT_EQ(0, disableCallCount) << "glDisable() shouldn't be called.";
}

}  // namespace
}  // namespace uinta
