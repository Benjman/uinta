#include "uinta/vbo.h"

#include <gtest/gtest.h>

#include "uinta/gl.h"
#include "uinta/mock/mock_gl.h"
#include "uinta/types.h"

namespace uinta {

struct VboTest : public testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(VboTest, Constructor) {
  bool onGenBuffersCalled = false;
  GLuint expId = 11;
  GLuint expCount = 1;
  gl.onGenBuffers = [&expId, &expCount, &onGenBuffersCalled](auto count,
                                                             auto* id) {
    *id = expId;
    ASSERT_EQ(expCount, count);
    onGenBuffersCalled = true;
  };
  GLenum target = GL_ARRAY_BUFFER;
  Vbo vbo(target, 0, &gl);
  ASSERT_EQ(expId, vbo.id());
  ASSERT_TRUE(onGenBuffersCalled);
}

TEST_F(VboTest, Destructor) {
  bool onDeleteBuffersCalled = false;
  GLuint expCount = 1;
  GLuint expId = 11;
  gl.onGenBuffers = [&expId](auto, auto* id) {
    auto i = expId;
    *id = i;
  };
  gl.onDeleteBuffers = [expCount, expId, &onDeleteBuffersCalled](auto count,
                                                                 auto* id) {
    ASSERT_EQ(expCount, count);
    ASSERT_EQ(expId, *id);
    onDeleteBuffersCalled = true;
  };
  {
    Vbo v(GL_ARRAY_BUFFER, 0, &gl);
  }
  ASSERT_TRUE(onDeleteBuffersCalled);
}

TEST_F(VboTest, ConstructorWithSize) {
  bool onBufferDataCalled = false;
  GLenum expTarget = GL_ARRAY_BUFFER;
  GLsizeiptr expSize = 1024;
  const void* expData = nullptr;
  GLenum expUsage = GL_STATIC_DRAW;
  gl.onBufferData = [&expTarget, &expSize, &expData, &expUsage,
                     &onBufferDataCalled](auto target, auto size,
                                          const auto* data, auto usage) {
    ASSERT_EQ(expTarget, target);
    ASSERT_EQ(expSize, size);
    ASSERT_EQ(expData, data);
    ASSERT_EQ(expUsage, usage);
    onBufferDataCalled = true;
  };
  Vbo vbo(expTarget, expSize, &gl);
  ASSERT_TRUE(onBufferDataCalled);
  ASSERT_EQ(expSize, vbo.capacity());
}

TEST_F(VboTest, Resize) {
  Vbo vbo(GL_ARRAY_BUFFER, 1, &gl);
  ASSERT_EQ(1, vbo.capacity());
  std::array<f32, 100> data;
  vbo.bufferData(data.data(), data.size() * sizeof(f32), GL_STATIC_DRAW);
  ASSERT_EQ(data.size() * sizeof(f32), vbo.capacity());
}

}  // namespace uinta
