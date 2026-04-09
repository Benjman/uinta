#include "uinta/vao.h"

#include <gtest/gtest.h>

#include <array>
#include <vector>

#include "uinta/mock/mock_gl.h"

namespace uinta {

// Vao tests
struct VaoTest : public testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(VaoTest, ConstructorCallsGenVertexArrays) {
  bool onGenVertexArraysCalled = false;
  GLuint expId = 13;
  GLsizei expCount = 1;
  gl.onGenVertexArrays = [&](GLsizei count, GLuint* id) {
    ASSERT_EQ(expCount, count);
    *id = expId;
    onGenVertexArraysCalled = true;
  };
  Vao vao(&gl);
  ASSERT_TRUE(onGenVertexArraysCalled);
  ASSERT_EQ(expId, vao.id());
}

TEST_F(VaoTest, DestructorCallsDeleteVertexArrays) {
  bool onDeleteVertexArraysCalled = false;
  GLuint expId = 42;
  GLsizei expCount = 1;
  gl.onGenVertexArrays = [&](GLsizei, GLuint* id) { *id = expId; };
  gl.onDeleteVertexArrays = [&](GLsizei count, GLuint* id) {
    ASSERT_EQ(expCount, count);
    ASSERT_EQ(expId, *id);
    onDeleteVertexArraysCalled = true;
  };
  {
    Vao vao(&gl);
  }
  ASSERT_TRUE(onDeleteVertexArraysCalled);
}

TEST_F(VaoTest, IdGetter) {
  GLuint expId = 99;
  gl.onGenVertexArrays = [&](GLsizei, GLuint* id) { *id = expId; };
  Vao vao(&gl);
  ASSERT_EQ(expId, vao.id());
}

TEST_F(VaoTest, LinkAttributeCallsVertexAttribPointer) {
  bool vertexAttribPointerCalled = false;
  Attribute attr{
      .index = 0,
      .size = 3,
      .type = GL_FLOAT,
      .normalized = GL_FALSE,
      .stride = 12,
      .offset = 0,
  };

  gl.onVertexAttribPointer = [&](GLuint index, GLint size, GLenum type,
                                 GLboolean normalized, GLsizei stride,
                                 const void* pointer) {
    ASSERT_EQ(attr.index, index);
    ASSERT_EQ(attr.size, size);
    ASSERT_EQ(attr.type, type);
    ASSERT_EQ(attr.normalized, normalized);
    ASSERT_EQ(attr.stride, stride);
    ASSERT_EQ(std::bit_cast<void*>(attr.offset), pointer);
    vertexAttribPointerCalled = true;
  };

  Vao vao(&gl);
  vao.linkAttribute(attr);
  ASSERT_TRUE(vertexAttribPointerCalled);
}

TEST_F(VaoTest, LinkAttributeCallsEnableVertexAttribArray) {
  bool enableVertexAttribArrayCalled = false;
  GLuint expIndex = 2;
  Attribute attr{
      .index = expIndex,
      .size = 3,
      .type = GL_FLOAT,
      .normalized = GL_FALSE,
      .stride = 12,
      .offset = 0,
  };

  gl.onEnableVertexAttribArray = [&](GLuint index) {
    ASSERT_EQ(expIndex, index);
    enableVertexAttribArrayCalled = true;
  };

  Vao vao(&gl);
  vao.linkAttribute(attr);
  ASSERT_TRUE(enableVertexAttribArrayCalled);
}

TEST_F(VaoTest, LinkAttributeWithOffset) {
  size_t expOffset = 24;
  const void* capturedPointer = nullptr;

  gl.onVertexAttribPointer = [&](GLuint, GLint, GLenum, GLboolean, GLsizei,
                                 const void* pointer) {
    capturedPointer = pointer;
  };

  Attribute attr{
      .index = 1,
      .size = 2,
      .type = GL_FLOAT,
      .normalized = GL_FALSE,
      .stride = 32,
      .offset = expOffset,
  };

  Vao vao(&gl);
  vao.linkAttribute(attr);
  ASSERT_EQ(std::bit_cast<void*>(expOffset), capturedPointer);
}

TEST_F(VaoTest, EboCreatesVboOnFirstCall) {
  bool genBuffersCalled = false;
  bool bindBufferCalled = false;
  GLenum boundTarget = GL_NONE;

  gl.onGenBuffers = [&](GLsizei, GLuint* id) {
    *id = 1;
    genBuffersCalled = true;
  };
  gl.onBindBuffer = [&](GLenum target, GLuint) {
    boundTarget = target;
    bindBufferCalled = true;
  };

  Vao vao(&gl);
  std::array<u32, 3> indices = {0, 1, 2};
  vao.ebo(std::span<u32>(indices));

  ASSERT_TRUE(genBuffersCalled);
  ASSERT_TRUE(bindBufferCalled);
  ASSERT_EQ(GL_ELEMENT_ARRAY_BUFFER, boundTarget);
}

TEST_F(VaoTest, EboReturnsBufferSegment) {
  gl.onGenBuffers = [&](GLsizei, GLuint* id) { *id = 1; };

  Vao vao(&gl);
  std::array<u32, 3> indices = {0, 1, 2};
  BufferSegment segment = vao.ebo(std::span<u32>(indices));

  ASSERT_EQ(0, segment.offset);
  ASSERT_EQ(indices.size() * sizeof(u32), static_cast<size_t>(segment.size));
}

TEST_F(VaoTest, EboWithOffsetCalculatesCorrectly) {
  gl.onGenBuffers = [&](GLsizei, GLuint* id) { *id = 1; };

  Vao vao(&gl);
  std::array<u32, 3> indices1 = {0, 1, 2};
  BufferSegment segment1 = vao.ebo(std::span<u32>(indices1));

  std::array<u32, 3> indices2 = {3, 4, 5};
  BufferSegment segment2 = vao.ebo(std::span<u32>(indices2), segment1);

  ASSERT_EQ(segment1.offset + segment1.size, segment2.offset);
  ASSERT_EQ(indices2.size() * sizeof(u32), static_cast<size_t>(segment2.size));
}

TEST_F(VaoTest, EboWithExplicitOffset) {
  gl.onGenBuffers = [&](GLsizei, GLuint* id) { *id = 1; };

  GLsizeiptr explicitOffset = 100;
  Vao vao(&gl);
  std::array<u32, 3> indices = {0, 1, 2};
  BufferSegment segment = vao.ebo(std::span<u32>(indices), explicitOffset);

  ASSERT_EQ(explicitOffset, segment.offset);
}

TEST_F(VaoTest, EboGetterReturnsNullBeforeFirstCall) {
  Vao vao(&gl);
  ASSERT_EQ(nullptr, vao.ebo());
}

TEST_F(VaoTest, EboGetterReturnsVboAfterFirstCall) {
  gl.onGenBuffers = [&](GLsizei, GLuint* id) { *id = 1; };

  Vao vao(&gl);
  std::array<u32, 3> indices = {0, 1, 2};
  vao.ebo(std::span<u32>(indices));

  ASSERT_NE(nullptr, vao.ebo());
}

TEST_F(VaoTest, EboReusesExistingVbo) {
  int genBuffersCallCount = 0;
  gl.onGenBuffers = [&](GLsizei, GLuint* id) {
    *id = 1;
    genBuffersCallCount++;
  };

  Vao vao(&gl);
  std::array<u32, 3> indices1 = {0, 1, 2};
  vao.ebo(std::span<u32>(indices1));
  int callsAfterFirst = genBuffersCallCount;

  std::array<u32, 3> indices2 = {3, 4, 5};
  vao.ebo(std::span<u32>(indices2));

  // Should not create a new buffer on second call
  ASSERT_EQ(callsAfterFirst, genBuffersCallCount);
}

// VaoGuard tests
struct VaoGuardTest : public testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(VaoGuardTest, ConstructorWithActiveTrueBindsVao) {
  GLuint boundId = 0;
  bool bindVertexArrayCalled = false;
  GLuint vaoId = 55;

  gl.onGenVertexArrays = [&](GLsizei, GLuint* id) { *id = vaoId; };
  gl.onBindVertexArray = [&](GLuint id) {
    boundId = id;
    bindVertexArrayCalled = true;
  };

  Vao vao(&gl);
  bindVertexArrayCalled = false;
  boundId = 0;

  VaoGuard guard(&vao, true);

  ASSERT_TRUE(bindVertexArrayCalled);
  ASSERT_EQ(vaoId, boundId);
}

TEST_F(VaoGuardTest, ConstructorWithActiveFalseDoesNotBind) {
  bool bindVertexArrayCalled = false;

  gl.onBindVertexArray = [&](GLuint) { bindVertexArrayCalled = true; };

  Vao vao(&gl);
  bindVertexArrayCalled = false;

  VaoGuard guard(&vao, false);

  ASSERT_FALSE(bindVertexArrayCalled);
}

TEST_F(VaoGuardTest, DestructorUnbindsWhenActive) {
  std::vector<GLuint> bindCalls;
  GLuint vaoId = 77;

  gl.onGenVertexArrays = [&](GLsizei, GLuint* id) { *id = vaoId; };
  gl.onBindVertexArray = [&](GLuint id) { bindCalls.push_back(id); };

  Vao vao(&gl);
  bindCalls.clear();

  {
    VaoGuard guard(&vao, true);
    ASSERT_EQ(1u, bindCalls.size());
    ASSERT_EQ(vaoId, bindCalls[0]);
  }

  ASSERT_EQ(2u, bindCalls.size());
  ASSERT_EQ(0u, bindCalls[1]);  // Unbind
}

TEST_F(VaoGuardTest, DestructorDoesNotUnbindWhenInactive) {
  bool bindVertexArrayCalled = false;

  gl.onBindVertexArray = [&](GLuint) { bindVertexArrayCalled = true; };

  Vao vao(&gl);
  bindVertexArrayCalled = false;

  {
    VaoGuard guard(&vao, false);
  }

  ASSERT_FALSE(bindVertexArrayCalled);
}

TEST_F(VaoGuardTest, ScopedUsagePattern) {
  std::vector<GLuint> bindCalls;
  GLuint vaoId = 99;

  gl.onGenVertexArrays = [&](GLsizei, GLuint* id) { *id = vaoId; };
  gl.onBindVertexArray = [&](GLuint id) { bindCalls.push_back(id); };

  Vao vao(&gl);
  bindCalls.clear();

  {
    VaoGuard guard(&vao, true);
    // Guard should have bound the VAO
  }
  // Guard destructor should have unbound the VAO

  ASSERT_EQ(2u, bindCalls.size());
  ASSERT_EQ(vaoId, bindCalls[0]);
  ASSERT_EQ(0u, bindCalls[1]);
}

TEST_F(VaoGuardTest, DefaultActiveIsTrue) {
  bool bindVertexArrayCalled = false;
  GLuint vaoId = 88;

  gl.onGenVertexArrays = [&](GLsizei, GLuint* id) { *id = vaoId; };
  gl.onBindVertexArray = [&](GLuint) { bindVertexArrayCalled = true; };

  Vao vao(&gl);
  bindVertexArrayCalled = false;

  VaoGuard guard(&vao);  // Default isActive = true

  ASSERT_TRUE(bindVertexArrayCalled);
}

TEST_F(VaoGuardTest, MultipleGuardsInSequence) {
  std::vector<GLuint> bindCalls;
  GLuint vaoId = 111;

  gl.onGenVertexArrays = [&](GLsizei, GLuint* id) { *id = vaoId; };
  gl.onBindVertexArray = [&](GLuint id) { bindCalls.push_back(id); };

  Vao vao(&gl);
  bindCalls.clear();

  {
    VaoGuard guard1(&vao, true);
  }
  {
    VaoGuard guard2(&vao, true);
  }

  ASSERT_EQ(4u, bindCalls.size());
  ASSERT_EQ(vaoId, bindCalls[0]);
  ASSERT_EQ(0u, bindCalls[1]);
  ASSERT_EQ(vaoId, bindCalls[2]);
  ASSERT_EQ(0u, bindCalls[3]);
}

}  // namespace uinta
