#include "uinta/post_process/passes/tonemap_pass.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

struct TonemapPassTest : public testing::Test {
 protected:
  MockOpenGLApi gl;

  void SetUp() override {
    gl.onCheckFramebufferStatus = [](auto) -> GLenum {
      return GL_FRAMEBUFFER_COMPLETE;
    };
    gl.onGetUniformLocation = [](auto, const GLchar*) -> GLint { return 1; };
    gl.onCreateShader = [](auto) -> GLuint { return 1; };
    gl.onCreateProgram = []() -> GLuint { return 1; };
    gl.onGetShaderiv = [](auto, auto, GLint* out) { *out = GL_TRUE; };
    gl.onGetProgramiv = [](auto, auto, GLint* out) { *out = GL_TRUE; };
  }
};

TEST_F(TonemapPassTest, ConstructsWithAgXDefault) {
  TonemapPass pass(&gl);
  EXPECT_EQ(pass.op(), TonemapPass::Operator::AgX);
  EXPECT_FLOAT_EQ(pass.exposure(), 1.0f);
  EXPECT_FALSE(pass.enabled());
}

TEST_F(TonemapPassTest, SetOperator) {
  TonemapPass pass(&gl);
  pass.op(TonemapPass::Operator::ACES);
  EXPECT_EQ(pass.op(), TonemapPass::Operator::ACES);
  pass.op(TonemapPass::Operator::Reinhard);
  EXPECT_EQ(pass.op(), TonemapPass::Operator::Reinhard);
}

TEST_F(TonemapPassTest, SetExposure) {
  TonemapPass pass(&gl);
  pass.exposure(2.5f);
  EXPECT_FLOAT_EQ(pass.exposure(), 2.5f);
}

TEST_F(TonemapPassTest, NameIsTonemap) {
  TonemapPass pass(&gl);
  EXPECT_EQ(pass.name(), "Tonemap");
}

TEST_F(TonemapPassTest, ExecuteBindsShaderAndDraws) {
  TonemapPass pass(&gl);

  // Fake minimal PassContext.
  Vao vao(&gl);
  Texture input(GL_TEXTURE_2D, 128, 128, 0, GL_LINEAR, GL_CLAMP_TO_EDGE, &gl);
  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.width = 128;
  ctx.height = 128;
  ctx.input = &input;
  ctx.output = nullptr;  // default FBO

  bool drewTriangle = false;
  gl.onDrawArrays = [&drewTriangle](GLenum mode, GLint first, GLsizei count) {
    drewTriangle = (mode == GL_TRIANGLES && first == 0 && count == 3);
  };

  pass.execute(ctx);
  EXPECT_TRUE(drewTriangle);
}

TEST_F(TonemapPassTest, ExecuteWithNullInputIsNoop) {
  TonemapPass pass(&gl);
  Vao vao(&gl);
  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.input = nullptr;

  bool drew = false;
  gl.onDrawArrays = [&drew](auto, auto, auto) { drew = true; };

  pass.execute(ctx);
  EXPECT_FALSE(drew);
}

}  // namespace uinta
