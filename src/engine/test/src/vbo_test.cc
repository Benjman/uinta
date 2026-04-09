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

TEST_F(VboTest, ResizeWithExistingBuffer) {
  GLuint originalId = 11;
  GLuint newId = 22;
  bool isFirstGenBuffers = true;
  gl.onGenBuffers = [&](auto, auto* id) {
    if (isFirstGenBuffers) {
      *id = originalId;
      isFirstGenBuffers = false;
    } else {
      *id = newId;
    }
  };

  std::vector<std::tuple<GLenum, GLuint>> bindCalls;
  gl.onBindBuffer = [&](auto target, auto id) {
    bindCalls.push_back({target, id});
  };

  bool copyBufferSubDataCalled = false;
  gl.onCopyBufferSubData = [&](auto readTarget, auto writeTarget,
                               auto readOffset, auto writeOffset, auto size) {
    ASSERT_EQ(GL_COPY_READ_BUFFER, readTarget);
    ASSERT_EQ(GL_COPY_WRITE_BUFFER, writeTarget);
    ASSERT_EQ(0, readOffset);
    ASSERT_EQ(0, writeOffset);
    ASSERT_EQ(1, size);  // original capacity
    copyBufferSubDataCalled = true;
  };

  GLuint deletedId = 0;
  gl.onDeleteBuffers = [&](auto, auto* id) { deletedId = *id; };

  Vbo vbo(GL_ARRAY_BUFFER, 1, &gl);
  ASSERT_EQ(1, vbo.capacity());
  ASSERT_EQ(originalId, vbo.id());

  bindCalls.clear();

  // Trigger resize with existing buffer
  VboGuard guard(&vbo, true);
  std::array<f32, 100> data;
  vbo.bufferData(data.data(), data.size() * sizeof(f32), GL_STATIC_DRAW);

  ASSERT_TRUE(copyBufferSubDataCalled);
  ASSERT_EQ(originalId, deletedId);
  ASSERT_EQ(newId, vbo.id());
  ASSERT_EQ(data.size() * sizeof(f32), vbo.capacity());
}

TEST_F(VboTest, TargetGetter) {
  GLenum expectedTarget = GL_ELEMENT_ARRAY_BUFFER;
  Vbo vbo(expectedTarget, 0, &gl);
  ASSERT_EQ(expectedTarget, vbo.target());
}

TEST_F(VboTest, SizeGetter) {
  Vbo vbo(GL_ARRAY_BUFFER, 0, &gl);
  ASSERT_EQ(0, vbo.size());

  VboGuard guard(&vbo, true);
  std::array<f32, 10> data;
  vbo.bufferData(data.data(), data.size() * sizeof(f32), GL_STATIC_DRAW);
  ASSERT_EQ(data.size() * sizeof(f32), vbo.size());
}

TEST_F(VboTest, BufferDataWithBufferSegmentOffset) {
  GLintptr lastOffset = -1;
  GLsizeiptr lastSize = -1;
  gl.onBufferSubData = [&](auto, auto offset, auto size, const auto*) {
    lastOffset = offset;
    lastSize = size;
  };

  Vbo vbo(GL_ARRAY_BUFFER, 1024, &gl);
  VboGuard guard(&vbo, true);

  // First buffer
  std::array<f32, 10> data1;
  BufferSegment segment1 =
      vbo.bufferData(data1.data(), data1.size() * sizeof(f32), GL_STATIC_DRAW);

  ASSERT_EQ(0, segment1.offset);
  ASSERT_EQ(data1.size() * sizeof(f32), segment1.size);
  ASSERT_EQ(0, lastOffset);

  // Second buffer using first segment as offset
  std::array<f32, 5> data2;
  BufferSegment segment2 = vbo.bufferData(
      data2.data(), data2.size() * sizeof(f32), GL_STATIC_DRAW, segment1);

  ASSERT_EQ(segment1.offset + segment1.size, segment2.offset);
  ASSERT_EQ(data2.size() * sizeof(f32), segment2.size);
  ASSERT_EQ(segment1.offset + segment1.size, lastOffset);
}

TEST_F(VboTest, ResizeSameSizeNoOp) {
  int bufferDataCallCount = 0;
  gl.onBufferData = [&](auto, auto, const auto*, auto) {
    bufferDataCallCount++;
  };

  Vbo vbo(GL_ARRAY_BUFFER, 100, &gl);
  ASSERT_EQ(1, bufferDataCallCount);  // Initial allocation

  VboGuard guard(&vbo, true);
  vbo.resize(100, GL_STATIC_DRAW);    // Same size, should be no-op
  ASSERT_EQ(1, bufferDataCallCount);  // No additional call
}

// VboGuard tests
struct VboGuardTest : public testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(VboGuardTest, ConstructorWithActiveTrue) {
  GLenum boundTarget = GL_NONE;
  GLuint boundId = 0;
  gl.onBindBuffer = [&](auto target, auto id) {
    boundTarget = target;
    boundId = id;
  };

  GLuint expectedId = 42;
  gl.onGenBuffers = [&](auto, auto* id) { *id = expectedId; };

  GLenum expectedTarget = GL_ARRAY_BUFFER;
  Vbo vbo(expectedTarget, 0, &gl);

  boundTarget = GL_NONE;
  boundId = 0;

  VboGuard guard(&vbo, true);

  ASSERT_EQ(expectedTarget, boundTarget);
  ASSERT_EQ(expectedId, boundId);
}

TEST_F(VboGuardTest, ConstructorWithActiveFalse) {
  bool bindBufferCalled = false;
  gl.onBindBuffer = [&](auto, auto) { bindBufferCalled = true; };

  Vbo vbo(GL_ARRAY_BUFFER, 0, &gl);
  bindBufferCalled = false;

  VboGuard guard(&vbo, false);

  ASSERT_FALSE(bindBufferCalled);
}

TEST_F(VboGuardTest, DestructorWhenActive) {
  std::vector<std::pair<GLenum, GLuint>> bindCalls;
  gl.onBindBuffer = [&](auto target, auto id) {
    bindCalls.push_back({target, id});
  };

  GLuint vboId = 55;
  gl.onGenBuffers = [&](auto, auto* id) { *id = vboId; };

  GLenum target = GL_ARRAY_BUFFER;
  Vbo vbo(target, 0, &gl);
  bindCalls.clear();

  {
    VboGuard guard(&vbo, true);
    ASSERT_EQ(1u, bindCalls.size());
    ASSERT_EQ(target, bindCalls[0].first);
    ASSERT_EQ(vboId, bindCalls[0].second);
  }

  ASSERT_EQ(2u, bindCalls.size());
  ASSERT_EQ(target, bindCalls[1].first);
  ASSERT_EQ(0u, bindCalls[1].second);  // Unbind
}

TEST_F(VboGuardTest, DestructorWhenInactive) {
  bool bindBufferCalled = false;
  gl.onBindBuffer = [&](auto, auto) { bindBufferCalled = true; };

  Vbo vbo(GL_ARRAY_BUFFER, 0, &gl);
  bindBufferCalled = false;

  {
    VboGuard guard(&vbo, false);
  }

  ASSERT_FALSE(bindBufferCalled);
}

TEST_F(VboGuardTest, ActivateMethod) {
  std::vector<std::pair<GLenum, GLuint>> bindCalls;
  gl.onBindBuffer = [&](auto target, auto id) {
    bindCalls.push_back({target, id});
  };

  GLuint vboId = 77;
  gl.onGenBuffers = [&](auto, auto* id) { *id = vboId; };

  GLenum target = GL_ELEMENT_ARRAY_BUFFER;
  Vbo vbo(target, 0, &gl);
  bindCalls.clear();

  VboGuard guard(&vbo, false);
  ASSERT_EQ(0u, bindCalls.size());

  guard.activate();
  ASSERT_EQ(1u, bindCalls.size());
  ASSERT_EQ(target, bindCalls[0].first);
  ASSERT_EQ(vboId, bindCalls[0].second);
}

TEST_F(VboGuardTest, ScopedUsage) {
  std::vector<std::pair<GLenum, GLuint>> bindCalls;
  gl.onBindBuffer = [&](auto target, auto id) {
    bindCalls.push_back({target, id});
  };

  GLuint vboId = 99;
  gl.onGenBuffers = [&](auto, auto* id) { *id = vboId; };

  GLenum target = GL_ARRAY_BUFFER;
  Vbo vbo(target, 0, &gl);
  bindCalls.clear();

  {
    VboGuard guard(&vbo, true);
    // Guard should have bound the buffer
  }
  // Guard destructor should have unbound the buffer

  ASSERT_EQ(2u, bindCalls.size());
  ASSERT_EQ(target, bindCalls[0].first);
  ASSERT_EQ(vboId, bindCalls[0].second);
  ASSERT_EQ(target, bindCalls[1].first);
  ASSERT_EQ(0u, bindCalls[1].second);
}

}  // namespace uinta
