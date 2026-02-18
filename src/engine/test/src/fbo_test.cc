#include "uinta/fbo.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"

namespace uinta {

struct FboTest : public testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(FboTest, Constructor) {
  bool onGenFramebuffersCalled = false;
  GLuint expId = 42;
  GLuint expCount = 1;
  gl.onGenFramebuffers = [&expId, &expCount, &onGenFramebuffersCalled](
      auto count, auto* id) {
    *id = expId;
    ASSERT_EQ(expCount, count);
    onGenFramebuffersCalled = true;
  };
  Fbo fbo(&gl);
  ASSERT_EQ(expId, fbo.id());
  ASSERT_TRUE(onGenFramebuffersCalled);
}

TEST_F(FboTest, Destructor) {
  bool onDeleteFramebuffersCalled = false;
  GLuint expCount = 1;
  GLuint expId = 42;
  gl.onGenFramebuffers = [&expId](auto, auto* id) { *id = expId; };
  gl.onDeleteFramebuffers = [expCount, expId, &onDeleteFramebuffersCalled](
      auto count, auto* id) {
    ASSERT_EQ(expCount, count);
    ASSERT_EQ(expId, *id);
    onDeleteFramebuffersCalled = true;
  };
  {
    Fbo fbo(&gl);
  }
  ASSERT_TRUE(onDeleteFramebuffersCalled);
}

TEST_F(FboTest, CheckStatus) {
  GLuint fboId = 42;
  GLenum expStatus = GL_FRAMEBUFFER_COMPLETE;

  gl.onGenFramebuffers = [&fboId](auto, auto* id) { *id = fboId; };
  gl.onCheckFramebufferStatus = [&expStatus](auto) { return expStatus; };

  Fbo fbo(&gl);
  ASSERT_EQ(expStatus, fbo.checkStatus());
}

TEST_F(FboTest, IsComplete) {
  GLuint fboId = 42;
  GLenum expStatus = GL_FRAMEBUFFER_COMPLETE;

  gl.onGenFramebuffers = [&fboId](auto, auto* id) { *id = fboId; };
  gl.onCheckFramebufferStatus = [&expStatus](auto) { return expStatus; };

  Fbo fbo(&gl);
  ASSERT_TRUE(fbo.isComplete());
}

TEST_F(FboTest, FboGuardActivates) {
  int bindCallCount = 0;
  GLuint fboId = 42;
  GLuint boundId = 0;

  gl.onGenFramebuffers = [&fboId](auto, auto* id) { *id = fboId; };
  gl.onBindFramebuffer = [&](auto, auto id) {
    bindCallCount++;
    if (bindCallCount == 1) {
      boundId = id;
    }
  };

  Fbo fbo(&gl);
  {
    FboGuard guard(&fbo, true);
  }
  // First call should bind the FBO, second call should unbind it
  ASSERT_EQ(2, bindCallCount);
  ASSERT_EQ(fboId, boundId);
}

TEST_F(FboTest, AttachColorRenderbuffer) {
  bool onFramebufferRenderbufferCalled = false;
  GLuint fboId = 42;
  GLuint rboId = 20;
  GLenum expAttachment = GL_COLOR_ATTACHMENT0;

  gl.onGenFramebuffers = [&fboId](auto, auto* id) { *id = fboId; };
  gl.onFramebufferRenderbuffer = [&](auto, auto attachment,
                                      auto, auto rbId) {
    ASSERT_EQ(expAttachment, attachment);
    ASSERT_EQ(rboId, rbId);
    onFramebufferRenderbufferCalled = true;
  };

  Fbo fbo(&gl);
  fbo.attachColorRenderbuffer(rboId, 0);
  ASSERT_TRUE(onFramebufferRenderbufferCalled);
}

}  // namespace uinta
