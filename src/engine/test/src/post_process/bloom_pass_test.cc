#include "uinta/post_process/passes/bloom_pass.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/post_process/resource_pool.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

struct BloomPassTest : public testing::Test {
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
    gl.onGetBooleanv = [](auto, GLboolean* out) { *out = GL_TRUE; };
    gl.onGetIntegerv = [](auto, GLint* out) { *out = 0; };
    gl.onIsEnabled = [](auto) -> GLboolean { return GL_FALSE; };
  }
};

TEST_F(BloomPassTest, ConstructorDefaults) {
  BloomPass pass(&gl);
  EXPECT_FALSE(pass.enabled());
  EXPECT_GT(pass.intensity(), 0.0f);
  EXPECT_GE(pass.threshold(), 0.0f);
  EXPECT_EQ(pass.mipCount(), BloomPass::kDefaultMipCount);
  EXPECT_EQ(pass.name(), "Bloom");
}

TEST_F(BloomPassTest, Setters) {
  BloomPass pass(&gl);
  pass.intensity(0.75f);
  pass.threshold(1.2f);
  pass.filterRadius(0.01f);
  pass.mipCount(6);
  EXPECT_FLOAT_EQ(pass.intensity(), 0.75f);
  EXPECT_FLOAT_EQ(pass.threshold(), 1.2f);
  EXPECT_FLOAT_EQ(pass.filterRadius(), 0.01f);
  EXPECT_EQ(pass.mipCount(), 6u);
}

TEST_F(BloomPassTest, ZeroIntensityBypasses) {
  BloomPass pass(&gl);
  pass.intensity(0.0f);

  Vao vao(&gl);
  Texture input(GL_TEXTURE_2D, 128, 128, 0, GL_LINEAR, GL_CLAMP_TO_EDGE, &gl);
  ResourcePool pool(&gl);

  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.width = 128;
  ctx.height = 128;
  ctx.input = &input;
  ctx.pool = &pool;
  // No blit helper means the pass should return silently in bypass.

  int drawCount = 0;
  gl.onDrawArrays = [&drawCount](auto, auto, auto) { drawCount++; };
  pass.execute(ctx);
  // With zero intensity and no blit helper, the early-out returns before
  // any draw.
  EXPECT_EQ(drawCount, 0);
}

TEST_F(BloomPassTest, NullInputIsNoop) {
  BloomPass pass(&gl);
  Vao vao(&gl);
  ResourcePool pool(&gl);

  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.input = nullptr;
  ctx.pool = &pool;

  int drawCount = 0;
  gl.onDrawArrays = [&drawCount](auto, auto, auto) { drawCount++; };
  pass.execute(ctx);
  EXPECT_EQ(drawCount, 0);
}

TEST_F(BloomPassTest, ExecuteDrawsDownsampleUpsampleAndComposite) {
  BloomPass pass(&gl);
  pass.mipCount(4);

  Vao vao(&gl);
  Texture input(GL_TEXTURE_2D, 256, 256, 0, GL_LINEAR, GL_CLAMP_TO_EDGE, &gl);
  ResourcePool pool(&gl);

  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.width = 256;
  ctx.height = 256;
  ctx.input = &input;
  ctx.pool = &pool;

  gl.onGetBooleanv = [](auto, GLboolean* out) { *out = GL_TRUE; };
  gl.onGetIntegerv = [](auto, GLint* out) { *out = 0; };
  gl.onIsEnabled = [](auto) -> GLboolean { return GL_FALSE; };

  int drawCount = 0;
  gl.onDrawArrays = [&drawCount](auto, auto, auto) { drawCount++; };

  pass.execute(ctx);

  // 4 downsamples + 3 upsamples + 1 composite = 8 draws.
  EXPECT_EQ(drawCount, 8);
}

}  // namespace uinta
