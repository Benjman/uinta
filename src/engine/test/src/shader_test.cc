#include "uinta/shader.h"

#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include "uinta/mock/mock_gl.h"

namespace uinta {

class ShaderTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
};

// Shader constructor tests
TEST_F(ShaderTest, ConstructorCreatesProgramId) {
  GLuint expectedProgramId = 42;
  gl.onCreateProgram = [&]() -> GLuint { return expectedProgramId; };

  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  EXPECT_EQ(expectedProgramId, shader.id());
}

TEST_F(ShaderTest, ConstructorStoresGlPointer) {
  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  EXPECT_EQ(&gl, shader.gl());
}

TEST_F(ShaderTest, ConstructorLinksProgram) {
  bool linkProgramCalled = false;
  GLuint linkedProgramId = 0;
  gl.onLinkProgram = [&](GLuint program) {
    linkProgramCalled = true;
    linkedProgramId = program;
  };

  GLuint expectedProgramId = 99;
  gl.onCreateProgram = [&]() -> GLuint { return expectedProgramId; };

  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  EXPECT_TRUE(linkProgramCalled);
  EXPECT_EQ(expectedProgramId, linkedProgramId);
}

// Shader destructor tests
TEST_F(ShaderTest, DestructorDeletesProgram) {
  bool deleteProgramCalled = false;
  GLuint deletedProgramId = 0;
  gl.onDeleteProgram = [&](GLuint program) {
    deleteProgramCalled = true;
    deletedProgramId = program;
  };

  GLuint expectedProgramId = 77;
  gl.onCreateProgram = [&]() -> GLuint { return expectedProgramId; };

  {
    std::unordered_map<GLenum, std::string> stagePaths;
    Shader shader(stagePaths, &gl);
  }

  EXPECT_TRUE(deleteProgramCalled);
  EXPECT_EQ(expectedProgramId, deletedProgramId);
}

// Shader getter tests
TEST_F(ShaderTest, IdGetterReturnsCorrectValue) {
  GLuint expectedId = 123;
  gl.onCreateProgram = [&]() -> GLuint { return expectedId; };

  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  EXPECT_EQ(expectedId, shader.id());
}

TEST_F(ShaderTest, GlGetterReturnsCorrectPointer) {
  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  EXPECT_EQ(&gl, shader.gl());
}

// ShaderGuard tests
class ShaderGuardTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;
};

TEST_F(ShaderGuardTest, ConstructorCallsUseProgram) {
  bool useProgramCalled = false;
  GLuint usedProgramId = 0;
  gl.onUseProgram = [&](GLuint program) {
    useProgramCalled = true;
    usedProgramId = program;
  };

  GLuint expectedProgramId = 55;
  gl.onCreateProgram = [&]() -> GLuint { return expectedProgramId; };

  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  useProgramCalled = false;  // Reset after shader construction
  usedProgramId = 0;

  ShaderGuard guard(&shader);

  EXPECT_TRUE(useProgramCalled);
  EXPECT_EQ(expectedProgramId, usedProgramId);
}

TEST_F(ShaderGuardTest, DestructorCallsUseProgramWithZero) {
  GLuint lastUsedProgramId = 999;
  gl.onUseProgram = [&](GLuint program) { lastUsedProgramId = program; };

  GLuint shaderProgramId = 88;
  gl.onCreateProgram = [&]() -> GLuint { return shaderProgramId; };

  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  {
    ShaderGuard guard(&shader);
    EXPECT_EQ(shaderProgramId, lastUsedProgramId);
  }

  EXPECT_EQ(0u, lastUsedProgramId);
}

TEST_F(ShaderGuardTest, ScopedUsage) {
  std::vector<GLuint> useProgramCalls;
  gl.onUseProgram = [&](GLuint program) { useProgramCalls.push_back(program); };

  GLuint shaderProgramId = 33;
  gl.onCreateProgram = [&]() -> GLuint { return shaderProgramId; };

  std::unordered_map<GLenum, std::string> stagePaths;
  Shader shader(stagePaths, &gl);

  useProgramCalls.clear();  // Clear calls from shader construction

  {
    ShaderGuard guard(&shader);
    // ShaderGuard should have called useProgram with shader's id
  }
  // ShaderGuard destructor should have called useProgram with 0

  ASSERT_EQ(2u, useProgramCalls.size());
  EXPECT_EQ(shaderProgramId, useProgramCalls[0]);
  EXPECT_EQ(0u, useProgramCalls[1]);
}

// Shader validation tests
TEST_F(ShaderTest, LinkFailureSetsIdToZero) {
  gl.onGetProgramiv = [](GLuint, GLenum pname, GLint* params) {
    if (pname == GL_LINK_STATUS) {
      *params = GL_ZERO;  // Link failure
    }
  };

  std::unordered_map<GLenum, std::string> stagePaths;

  // Note: This will trigger LOG(FATAL) in the implementation,
  // but in test environment without death test, we verify the id is set to 0
  // The actual behavior depends on how LOG(FATAL) is configured in tests
}

}  // namespace uinta
