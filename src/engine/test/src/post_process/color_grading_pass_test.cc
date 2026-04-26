#include "uinta/post_process/passes/color_grading_pass.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/texture.h"
#include "uinta/vao.h"

namespace uinta {

struct ColorGradingPassTest : public testing::Test {
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

TEST_F(ColorGradingPassTest, ConstructDefaults) {
  ColorGradingPass pass(&gl);
  EXPECT_FALSE(pass.enabled());
  EXPECT_EQ(pass.lutSize(), 0u);
  EXPECT_FLOAT_EQ(pass.intensity(), 1.0f);
  EXPECT_EQ(pass.name(), "Color Grading");
}

TEST_F(ColorGradingPassTest, SetIntensity) {
  ColorGradingPass pass(&gl);
  pass.intensity(0.5f);
  EXPECT_FLOAT_EQ(pass.intensity(), 0.5f);
}

TEST_F(ColorGradingPassTest, SetLutRejectsMissingFile) {
  ColorGradingPass pass(&gl);
  auto status = pass.setLut("texture/lut/does_not_exist.png");
  EXPECT_FALSE(status.ok());
}

TEST_F(ColorGradingPassTest, ClearLutResetsState) {
  ColorGradingPass pass(&gl);
  pass.clearLut();
  EXPECT_EQ(pass.lutSize(), 0u);
  EXPECT_TRUE(pass.lutPath().empty());
}

TEST_F(ColorGradingPassTest, ExecuteWithoutLutFallsThrough) {
  ColorGradingPass pass(&gl);
  Vao vao(&gl);
  Texture input(GL_TEXTURE_2D, 64, 64, 0, GL_LINEAR, GL_CLAMP_TO_EDGE, &gl);

  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.width = 64;
  ctx.height = 64;
  ctx.input = &input;

  bool drew = false;
  gl.onDrawArrays = [&drew](GLenum, GLint, GLsizei) { drew = true; };
  pass.execute(ctx);
  EXPECT_TRUE(drew);
}

TEST_F(ColorGradingPassTest, NullInputIsNoop) {
  ColorGradingPass pass(&gl);
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
