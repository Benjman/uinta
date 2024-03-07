#include "uinta/vbo.h"

#include <gtest/gtest.h>

#include "./mock_gl.h"

namespace uinta {

TEST(Vbo, constructor) {
  MockOpenGLApi api;
  bool onGenBuffersCalled = false;
  GLuint expId = 11;
  api.onGenBuffers = [&](GLuint, GLuint* id) {
    *id = expId;
    onGenBuffersCalled = true;
  };
  GLenum target = GL_ARRAY_BUFFER;
  Vbo vbo(target, 0, &api);
  ASSERT_EQ(expId, vbo.id());
  ASSERT_TRUE(onGenBuffersCalled);
}

TEST(Vbo, constructor_withInitialSize) {
  // TODO:
}

TEST(Vbo, destructor) {
  MockOpenGLApi api;
  bool onDeleteBuffersCalled = false;
  GLuint expCount = 1;
  GLuint expId = 11;
  api.onGenBuffers = [&](GLuint, GLuint* id) { *id = expId; };
  api.onDeleteBuffers = [&](GLuint count, GLuint* id) {
    ASSERT_EQ(expCount, count);
    ASSERT_EQ(expId, *id);
    onDeleteBuffersCalled = true;
  };
  { Vbo v(GL_ARRAY_BUFFER, 0, &api); }
  ASSERT_TRUE(onDeleteBuffersCalled);
}

TEST(Vbo, constructorWithSize) {
  MockOpenGLApi api;
  bool onBufferDataCalled = false;
  GLenum expTarget = GL_ARRAY_BUFFER;
  GLsizeiptr expSize = 1024;
  const void* expData = nullptr;
  GLenum expUsage = GL_STATIC_DRAW;
  api.onBufferData = [&](GLenum target, GLsizeiptr size, const void* data,
                         GLenum usage) {
    ASSERT_EQ(expTarget, target);
    ASSERT_EQ(expSize, size);
    ASSERT_EQ(expData, data);
    ASSERT_EQ(expUsage, usage);
    onBufferDataCalled = true;
  };
  Vbo vbo(expTarget, expSize, &api);
  ASSERT_TRUE(onBufferDataCalled);
  ASSERT_EQ(expSize, vbo.size());
}

}  // namespace uinta
