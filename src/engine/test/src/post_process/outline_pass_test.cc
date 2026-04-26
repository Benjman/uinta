#include "uinta/post_process/passes/outline_pass.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"
#include "uinta/post_process/pass.h"
#include "uinta/post_process/render_target.h"
#include "uinta/vao.h"

namespace uinta {

struct OutlinePassTest : public testing::Test {
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

TEST_F(OutlinePassTest, DisabledByDefault) {
  OutlinePass pass(&gl);
  EXPECT_TRUE(pass.enabled());
  EXPECT_EQ(pass.name(), "Outline");
}

TEST_F(OutlinePassTest, Setters) {
  OutlinePass pass(&gl);
  pass.color(glm::vec3(0.1f, 0.2f, 0.3f));
  pass.thickness(2.0f);
  pass.depthSensitivity(50.0f);
  pass.normalSensitivity(3.0f);
  EXPECT_FLOAT_EQ(pass.color().g, 0.2f);
  EXPECT_FLOAT_EQ(pass.thickness(), 2.0f);
  EXPECT_FLOAT_EQ(pass.depthSensitivity(), 50.0f);
  EXPECT_FLOAT_EQ(pass.normalSensitivity(), 3.0f);
}

TEST_F(OutlinePassTest, RequiresSceneDepth) {
  OutlinePass pass(&gl);
  Vao vao(&gl);
  Texture input(GL_TEXTURE_2D, 64, 64, 0, GL_LINEAR, GL_CLAMP_TO_EDGE, &gl);

  // No depth.
  RenderTarget noDepth(&gl, 64, 64, RenderTargetFormats::Hdr());
  PassContext ctx;
  ctx.gl = &gl;
  ctx.fullscreenVao = &vao;
  ctx.input = &input;
  ctx.sceneTarget = &noDepth;
  ctx.width = 64;
  ctx.height = 64;

  bool drew = false;
  gl.onDrawArrays = [&drew](auto, auto, auto) { drew = true; };
  pass.execute(ctx);
  EXPECT_FALSE(drew);
}

TEST_F(OutlinePassTest, ExecutesWithDepth) {
  OutlinePass pass(&gl);
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
