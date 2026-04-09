#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"

namespace uinta {
namespace {

class CullFaceGuardTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;

  GLenum isEnabledCalledCap = GL_NONE;
  GLenum enableCalledCap = GL_NONE;
  GLenum disableCalledCap = GL_NONE;
  GLenum cullFaceCalledMode = GL_NONE;
  GLenum getIntegervCalledPname = GL_NONE;
  int cullFaceCallCount = 0;
  GLenum prevCullFaceMode = GL_BACK;

  void SetUp() override {
    gl.onIsEnabled = [&](auto capability) {
      isEnabledCalledCap = capability;
      return false;
    };
    gl.onEnable = [&](auto capability) { enableCalledCap = capability; };
    gl.onDisable = [&](auto capability) { disableCalledCap = capability; };
    gl.onCullFace = [&](auto mode) {
      cullFaceCalledMode = mode;
      cullFaceCallCount++;
    };
    gl.onGetIntegerv = [&](auto pname, auto data) {
      getIntegervCalledPname = pname;
      *data = static_cast<GLint>(prevCullFaceMode);
    };
  }
};

TEST_F(CullFaceGuardTest, UsesCullFaceCapability) {
  {
    CullFaceGuard guard(GL_BACK, true, &gl);
    ASSERT_EQ(GL_CULL_FACE, isEnabledCalledCap);
    ASSERT_EQ(GL_CULL_FACE, enableCalledCap);
  }
  ASSERT_EQ(GL_CULL_FACE, disableCalledCap);
}

TEST_F(CullFaceGuardTest, QueriesPreviousCullFaceMode) {
  CullFaceGuard guard(GL_BACK, true, &gl);
  ASSERT_EQ(GL_CULL_FACE_MODE, getIntegervCalledPname);
}

TEST_F(CullFaceGuardTest, SetsCullFaceModeWhenDifferent) {
  prevCullFaceMode = GL_FRONT;
  CullFaceGuard guard(GL_BACK, true, &gl);
  ASSERT_EQ(GL_BACK, cullFaceCalledMode);
  ASSERT_EQ(1, cullFaceCallCount);
}

TEST_F(CullFaceGuardTest, DoesNotSetCullFaceModeWhenSame) {
  prevCullFaceMode = GL_BACK;
  CullFaceGuard guard(GL_BACK, true, &gl);
  ASSERT_EQ(0, cullFaceCallCount);
}

TEST_F(CullFaceGuardTest, RestoresPreviousModeOnDestruction) {
  prevCullFaceMode = GL_FRONT;
  {
    CullFaceGuard guard(GL_BACK, true, &gl);
    cullFaceCalledMode = GL_NONE;
  }
  ASSERT_EQ(GL_FRONT, cullFaceCalledMode);
}

TEST_F(CullFaceGuardTest, DoesNotRestoreWhenModeUnchanged) {
  prevCullFaceMode = GL_BACK;
  {
    CullFaceGuard guard(GL_BACK, true, &gl);
  }
  ASSERT_EQ(0, cullFaceCallCount);
}

TEST_F(CullFaceGuardTest, InactiveDoesNotSetMode) {
  prevCullFaceMode = GL_FRONT;
  {
    CullFaceGuard guard(GL_BACK, false, &gl);
    ASSERT_EQ(0, cullFaceCallCount);
  }
}

TEST_F(CullFaceGuardTest, FrontMode) {
  prevCullFaceMode = GL_BACK;
  CullFaceGuard guard(GL_FRONT, true, &gl);
  ASSERT_EQ(GL_FRONT, cullFaceCalledMode);
}

TEST_F(CullFaceGuardTest, FrontAndBackMode) {
  prevCullFaceMode = GL_BACK;
  CullFaceGuard guard(GL_FRONT_AND_BACK, true, &gl);
  ASSERT_EQ(GL_FRONT_AND_BACK, cullFaceCalledMode);
}

TEST_F(CullFaceGuardTest, DefaultModeIsBack) {
  prevCullFaceMode = GL_FRONT;
  CullFaceGuard guard(GL_BACK, true, &gl);
  ASSERT_EQ(GL_BACK, cullFaceCalledMode);
}

}  // namespace
}  // namespace uinta
