#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"

namespace uinta {
namespace {

class PolygonModeTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;

  GLenum polygonModeFace = GL_NONE;
  GLenum polygonModeMode = GL_NONE;
  int polygonModeCallCount = 0;

  void SetUp() override {
    gl.onPolygonMode = [&](auto face, auto mode) {
      polygonModeFace = face;
      polygonModeMode = mode;
      polygonModeCallCount++;
    };
  }
};

TEST_F(PolygonModeTest, ConstructorSetsMode) {
  PolygonMode pm(GL_LINE, &gl);
  ASSERT_EQ(GL_FRONT_AND_BACK, polygonModeFace);
  ASSERT_EQ(GL_LINE, polygonModeMode);
}

TEST_F(PolygonModeTest, DestructorRestoresFill) {
  {
    PolygonMode pm(GL_LINE, &gl);
    polygonModeFace = GL_NONE;
    polygonModeMode = GL_NONE;
  }
  ASSERT_EQ(GL_FRONT_AND_BACK, polygonModeFace);
  ASSERT_EQ(GL_FILL, polygonModeMode);
}

TEST_F(PolygonModeTest, FillMode) {
  PolygonMode pm(GL_FILL, &gl);
  ASSERT_EQ(GL_FILL, polygonModeMode);
}

TEST_F(PolygonModeTest, LineMode) {
  PolygonMode pm(GL_LINE, &gl);
  ASSERT_EQ(GL_LINE, polygonModeMode);
}

TEST_F(PolygonModeTest, PointMode) {
  PolygonMode pm(GL_POINT, &gl);
  ASSERT_EQ(GL_POINT, polygonModeMode);
}

TEST_F(PolygonModeTest, AlwaysUsesFrontAndBack) {
  PolygonMode pm(GL_LINE, &gl);
  ASSERT_EQ(GL_FRONT_AND_BACK, polygonModeFace);
}

TEST_F(PolygonModeTest, CallsPolygonModeTwice) {
  {
    PolygonMode pm(GL_LINE, &gl);
    ASSERT_EQ(1, polygonModeCallCount);
  }
  ASSERT_EQ(2, polygonModeCallCount);
}

TEST_F(PolygonModeTest, DefaultModeIsFill) {
  PolygonMode pm(GL_FILL, &gl);
  ASSERT_EQ(GL_FILL, polygonModeMode);
}

}  // namespace
}  // namespace uinta
