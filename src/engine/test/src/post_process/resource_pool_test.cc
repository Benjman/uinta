#include "uinta/post_process/resource_pool.h"

#include <gtest/gtest.h>

#include "uinta/mock/mock_gl.h"

namespace uinta {

struct ResourcePoolTest : public testing::Test {
 protected:
  MockOpenGLApi gl;
  void SetUp() override {
    gl.onCheckFramebufferStatus = [](auto) -> GLenum {
      return GL_FRAMEBUFFER_COMPLETE;
    };
  }
};

namespace {
ResourceDesc MakeDesc(u32 w, u32 h) noexcept {
  ResourceDesc d;
  d.width = w;
  d.height = h;
  d.color = RenderTargetFormats::Hdr();
  d.debugName = "test";
  return d;
}
}  // namespace

TEST_F(ResourcePoolTest, AcquireAllocatesFirstTime) {
  ResourcePool pool(&gl);
  EXPECT_EQ(pool.size(), 0u);
  auto* rt = pool.acquire(MakeDesc(256, 128));
  ASSERT_NE(rt, nullptr);
  EXPECT_EQ(pool.size(), 1u);
  EXPECT_EQ(rt->width(), 256u);
  EXPECT_EQ(rt->height(), 128u);
}

TEST_F(ResourcePoolTest, AcquireWhileInUseAllocatesAnother) {
  ResourcePool pool(&gl);
  auto* a = pool.acquire(MakeDesc(256, 256));
  auto* b = pool.acquire(MakeDesc(256, 256));
  EXPECT_NE(a, b);
  EXPECT_EQ(pool.size(), 2u);
}

TEST_F(ResourcePoolTest, ReleaseMakesReusable) {
  ResourcePool pool(&gl);
  auto* a = pool.acquire(MakeDesc(64, 64));
  pool.release(a);
  auto* b = pool.acquire(MakeDesc(64, 64));
  EXPECT_EQ(a, b);
  EXPECT_EQ(pool.size(), 1u);
}

TEST_F(ResourcePoolTest, DifferentDescNoReuse) {
  ResourcePool pool(&gl);
  auto* a = pool.acquire(MakeDesc(64, 64));
  pool.release(a);
  auto* b = pool.acquire(MakeDesc(128, 128));
  EXPECT_NE(a, b);
  EXPECT_EQ(pool.size(), 2u);
}

TEST_F(ResourcePoolTest, ResetActiveFreesAll) {
  ResourcePool pool(&gl);
  auto* a = pool.acquire(MakeDesc(64, 64));
  auto* b = pool.acquire(MakeDesc(64, 64));
  (void)a;
  (void)b;
  pool.resetActive();
  // After resetActive both are free; re-acquire should reuse, not allocate.
  auto* c = pool.acquire(MakeDesc(64, 64));
  auto* d = pool.acquire(MakeDesc(64, 64));
  EXPECT_EQ(pool.size(), 2u);
  // c and d should be drawn from the pool (a & b, in some order).
  EXPECT_TRUE(c == a || c == b);
  EXPECT_TRUE(d == a || d == b);
  EXPECT_NE(c, d);
}

TEST_F(ResourcePoolTest, InvalidateDropsEntries) {
  ResourcePool pool(&gl);
  pool.acquire(MakeDesc(64, 64));
  pool.acquire(MakeDesc(32, 32));
  EXPECT_EQ(pool.size(), 2u);
  pool.invalidate();
  EXPECT_EQ(pool.size(), 0u);
}

TEST_F(ResourcePoolTest, DescMatchesRespectsMips) {
  ResourcePool pool(&gl);
  ResourceDesc d1 = MakeDesc(256, 256);
  d1.color.levels = 1;
  ResourceDesc d2 = d1;
  d2.color.levels = 5;

  auto* a = pool.acquire(d1);
  pool.release(a);
  auto* b = pool.acquire(d2);
  EXPECT_NE(a, b);  // different mip counts => no reuse
  EXPECT_EQ(pool.size(), 2u);
}

TEST_F(ResourcePoolTest, DescMatchesRespectsDepth) {
  ResourcePool pool(&gl);
  ResourceDesc noDepth = MakeDesc(64, 64);
  ResourceDesc withDepth = MakeDesc(64, 64);
  withDepth.depth = RenderTargetFormats::Depth24();

  auto* a = pool.acquire(noDepth);
  pool.release(a);
  auto* b = pool.acquire(withDepth);
  EXPECT_NE(a, b);
}

}  // namespace uinta
