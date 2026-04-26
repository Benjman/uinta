#include "uinta/post_process/render_target.h"

#include <gtest/gtest.h>

#include <vector>

#include "uinta/mock/mock_gl.h"

namespace uinta {

struct RenderTargetTest : public testing::Test {
 protected:
  MockOpenGLApi gl;

  void SetUp() override {
    // Default: every GL call is a no-op. Supply default completeness so
    // RenderTarget's isComplete() probe doesn't spam errors.
    gl.onCheckFramebufferStatus = [](auto) -> GLenum {
      return GL_FRAMEBUFFER_COMPLETE;
    };
  }
};

TEST_F(RenderTargetTest, AllocatesColorAndFbo) {
  bool fbGenerated = false;
  bool colorTexGenerated = false;
  gl.onGenFramebuffers = [&fbGenerated](auto, auto* ids) {
    *ids = 42;
    fbGenerated = true;
  };
  gl.onGenTextures = [&colorTexGenerated](auto, auto* ids) {
    *ids = 100;
    colorTexGenerated = true;
  };

  RenderTarget rt(&gl, 256, 128, RenderTargetFormats::Hdr());

  EXPECT_TRUE(fbGenerated);
  EXPECT_TRUE(colorTexGenerated);
  EXPECT_NE(rt.color(), nullptr);
  EXPECT_EQ(rt.depth(), nullptr);  // No depth requested.
  EXPECT_NE(rt.fbo(), nullptr);
  EXPECT_EQ(rt.width(), 256u);
  EXPECT_EQ(rt.height(), 128u);
}

TEST_F(RenderTargetTest, AllocatesDepthWhenRequested) {
  std::vector<GLuint> generatedTextures;
  gl.onGenTextures = [&generatedTextures](auto, auto* ids) {
    *ids = static_cast<GLuint>(100 + generatedTextures.size());
    generatedTextures.push_back(*ids);
  };

  RenderTarget rt(&gl, 64, 64, RenderTargetFormats::Hdr(),
                  RenderTargetFormats::Depth24());

  EXPECT_NE(rt.color(), nullptr);
  EXPECT_NE(rt.depth(), nullptr);
  EXPECT_GE(generatedTextures.size(), 2u);
}

TEST_F(RenderTargetTest, ZeroDimensionsDefersAllocation) {
  bool texGenerated = false;
  gl.onGenTextures = [&texGenerated](auto, auto*) { texGenerated = true; };
  RenderTarget rt(&gl, 0, 0, RenderTargetFormats::Hdr());
  EXPECT_FALSE(texGenerated);
  EXPECT_EQ(rt.color(), nullptr);
  EXPECT_EQ(rt.fbo(), nullptr);
}

TEST_F(RenderTargetTest, ResizeReallocates) {
  gl.onGenTextures = [](auto, auto* ids) { *ids = 1; };
  RenderTarget rt(&gl, 128, 128, RenderTargetFormats::Hdr());
  ASSERT_NE(rt.color(), nullptr);

  rt.resize(256, 256);
  EXPECT_EQ(rt.width(), 256u);
  EXPECT_EQ(rt.height(), 256u);
  EXPECT_NE(rt.color(), nullptr);
}

TEST_F(RenderTargetTest, ResizeSameDimensionsNoop) {
  int textureGenCount = 0;
  gl.onGenTextures = [&textureGenCount](auto, auto* ids) {
    *ids = 1;
    textureGenCount++;
  };
  RenderTarget rt(&gl, 128, 128, RenderTargetFormats::Hdr());
  int countAfterInit = textureGenCount;
  rt.resize(128, 128);
  EXPECT_EQ(textureGenCount, countAfterInit);
}

TEST_F(RenderTargetTest, WidthHeightPerMip) {
  TextureFormat fmt = RenderTargetFormats::Hdr();
  fmt.levels = 4;
  gl.onGenTextures = [](auto, auto* ids) { *ids = 1; };

  RenderTarget rt(&gl, 256, 128, fmt);
  EXPECT_EQ(rt.width(0), 256u);
  EXPECT_EQ(rt.height(0), 128u);
  EXPECT_EQ(rt.width(1), 128u);
  EXPECT_EQ(rt.height(1), 64u);
  EXPECT_EQ(rt.width(2), 64u);
  EXPECT_EQ(rt.height(2), 32u);
  EXPECT_EQ(rt.width(3), 32u);
  EXPECT_EQ(rt.height(3), 16u);
}

TEST_F(RenderTargetTest, MipChainCreatesFboPerLevel) {
  TextureFormat fmt = RenderTargetFormats::Hdr();
  fmt.levels = 5;
  int fboGenCount = 0;
  gl.onGenFramebuffers = [&fboGenCount](auto, auto* ids) {
    *ids = static_cast<GLuint>(1 + fboGenCount);
    fboGenCount++;
  };
  gl.onGenTextures = [](auto, auto* ids) { *ids = 1; };

  RenderTarget rt(&gl, 256, 256, fmt);

  EXPECT_EQ(rt.mipCount(), 5u);
  for (u32 m = 0; m < 5; ++m) {
    EXPECT_NE(rt.fbo(m), nullptr) << "mip " << m;
  }
  EXPECT_EQ(rt.fbo(5), nullptr);
  // At minimum one FBO per mip.
  EXPECT_GE(fboGenCount, 5);
}

TEST_F(RenderTargetTest, DebugNameStored) {
  RenderTarget rt(&gl, 64, 64, RenderTargetFormats::Hdr(), std::nullopt,
                  "test-target");
  EXPECT_EQ(rt.debugName(), "test-target");
}

}  // namespace uinta
