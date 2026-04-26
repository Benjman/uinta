#include "uinta/post_process/passes/fog_pass.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/post_process/render_target.h"
#include "uinta/vao.h"

namespace uinta {

struct FogPassTest : public testing::Test {
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

TEST_F(FogPassTest, DisabledByDefault) {
  FogPass pass(&gl);
  EXPECT_TRUE(pass.enabled());
  EXPECT_EQ(pass.name(), "Fog");
}

TEST_F(FogPassTest, Setters) {
  FogPass pass(&gl);
  pass.density(0.02f);
  pass.color(glm::vec3(0.1f, 0.2f, 0.3f));
  pass.farFadeStart(0.75f);
  EXPECT_FLOAT_EQ(pass.density(), 0.02f);
  EXPECT_FLOAT_EQ(pass.color().r, 0.1f);
  EXPECT_FLOAT_EQ(pass.farFadeStart(), 0.75f);
}

TEST_F(FogPassTest, NullInputIsNoop) {
  FogPass pass(&gl);
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

TEST_F(FogPassTest, NoSceneDepthIsNoop) {
  FogPass pass(&gl);
  Vao vao(&gl);
  Texture input(GL_TEXTURE_2D, 64, 64, 0, GL_LINEAR, GL_CLAMP_TO_EDGE, &gl);

  // A render target without depth attachment.
  RenderTarget rt(&gl, 64, 64, RenderTargetFormats::Hdr());
  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.input = &input;
  ctx.sceneTarget = &rt;

  bool drew = false;
  gl.onDrawArrays = [&drew](auto, auto, auto) { drew = true; };
  pass.execute(ctx);
  EXPECT_FALSE(drew);
}

TEST_F(FogPassTest, ExecutesWhenSceneDepthPresent) {
  FogPass pass(&gl);
  Vao vao(&gl);
  Texture input(GL_TEXTURE_2D, 64, 64, 0, GL_LINEAR, GL_CLAMP_TO_EDGE, &gl);

  RenderTarget rt(&gl, 64, 64, RenderTargetFormats::Hdr(),
                  RenderTargetFormats::Depth24());
  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.width = 64;
  ctx.height = 64;
  ctx.input = &input;
  ctx.sceneTarget = &rt;

  bool drew = false;
  gl.onDrawArrays = [&drew](auto, auto, auto) { drew = true; };
  pass.execute(ctx);
  EXPECT_TRUE(drew);
}

}  // namespace uinta
