#include "uinta/uniform.h"

#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

#include "uinta/mock/mock_gl.h"
#include "uinta/shader.h"

namespace uinta {

// Base test fixture for all Uniform tests
class UniformTest : public ::testing::Test {
 protected:
  MockOpenGLApi gl;

  // Helper to create a shader with the mock GL API
  Shader createShader() {
    std::unordered_map<GLenum, std::string> stagePaths;
    return Shader{stagePaths, &gl};
  }
};

// ============================================================================
// Uniform base class constructor tests
// ============================================================================

TEST_F(UniformTest, ConstructorCallsGetUniformLocation) {
  GLuint shaderId = 42;
  std::string uniformName = "testUniform";
  GLint expectedLocation = 5;
  bool getUniformLocationCalled = false;

  gl.onCreateProgram = [&]() -> GLuint { return shaderId; };
  gl.onGetUniformLocation = [&](GLuint program, const GLchar* name) -> GLint {
    getUniformLocationCalled = true;
    EXPECT_EQ(shaderId, program);
    EXPECT_STREQ(uniformName.c_str(), name);
    return expectedLocation;
  };

  Shader shader = createShader();
  Uniform1f uniform(uniformName, &shader);

  EXPECT_TRUE(getUniformLocationCalled);
  EXPECT_EQ(expectedLocation, uniform.location());
}

TEST_F(UniformTest, ConstructorWithInvalidLocationReturnsMinusOne) {
  GLuint shaderId = 42;
  GLint invalidLocation = -1;

  gl.onCreateProgram = [&]() -> GLuint { return shaderId; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return invalidLocation;
  };

  Shader shader = createShader();
  Uniform1f uniform("nonExistent", &shader);

  EXPECT_EQ(invalidLocation, uniform.location());
}

TEST_F(UniformTest, LocationGetterReturnsCorrectValue) {
  GLint expectedLocation = 10;
  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return expectedLocation;
  };

  Shader shader = createShader();
  Uniform1f uniform("test", &shader);

  EXPECT_EQ(expectedLocation, uniform.location());
}

TEST_F(UniformTest, ValueGetterReturnsValueAfterAssignment) {
  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return 1; };

  Shader shader = createShader();
  Uniform1f uniform("test", &shader);
  uniform = 42.5f;

  EXPECT_EQ(42.5f, uniform.value());
}

// ============================================================================
// Uniform1f tests
// ============================================================================

TEST_F(UniformTest, Uniform1f_AssignmentCallsGlUniform1f) {
  GLint location = 3;
  GLfloat expectedValue = 1.5f;
  bool uniform1fCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform1f = [&](GLint loc, GLfloat v) {
    uniform1fCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue, v);
  };

  Shader shader = createShader();
  Uniform1f uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform1fCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

TEST_F(UniformTest, Uniform1f_AssignmentSkippedWhenLocationInvalid) {
  bool uniform1fCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return -1; };
  gl.onUniform1f = [&](GLint, GLfloat) { uniform1fCalled = true; };

  Shader shader = createShader();
  Uniform1f uniform("test", &shader);
  uniform = 1.0f;

  EXPECT_FALSE(uniform1fCalled);
}

TEST_F(UniformTest, Uniform1f_AssignmentSkippedWhenValueUnchanged) {
  int callCount = 0;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return 1; };
  gl.onUniform1f = [&](GLint, GLfloat) { callCount++; };

  Shader shader = createShader();
  Uniform1f uniform("test", &shader);
  uniform = 1.0f;
  uniform = 1.0f;  // Same value - should not call GL

  EXPECT_EQ(1, callCount);
}

// ============================================================================
// Uniform2f tests
// ============================================================================

TEST_F(UniformTest, Uniform2f_AssignmentCallsGlUniform2f) {
  GLint location = 4;
  glm::vec2 expectedValue(1.0f, 2.0f);
  bool uniform2fCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform2f = [&](GLint loc, GLfloat v0, GLfloat v1) {
    uniform2fCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
  };

  Shader shader = createShader();
  Uniform2f uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform2fCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

TEST_F(UniformTest, Uniform2f_AssignmentSkippedWhenLocationInvalid) {
  bool uniform2fCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return -1; };
  gl.onUniform2f = [&](GLint, GLfloat, GLfloat) { uniform2fCalled = true; };

  Shader shader = createShader();
  Uniform2f uniform("test", &shader);
  uniform = glm::vec2(1.0f, 2.0f);

  EXPECT_FALSE(uniform2fCalled);
}

// ============================================================================
// Uniform3f tests
// ============================================================================

TEST_F(UniformTest, Uniform3f_AssignmentCallsGlUniform3f) {
  GLint location = 5;
  glm::vec3 expectedValue(1.0f, 2.0f, 3.0f);
  bool uniform3fCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform3f = [&](GLint loc, GLfloat v0, GLfloat v1, GLfloat v2) {
    uniform3fCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
    EXPECT_EQ(expectedValue.z, v2);
  };

  Shader shader = createShader();
  Uniform3f uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform3fCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform4f tests
// ============================================================================

TEST_F(UniformTest, Uniform4f_AssignmentCallsGlUniform4f) {
  GLint location = 6;
  glm::vec4 expectedValue(1.0f, 2.0f, 3.0f, 4.0f);
  bool uniform4fCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform4f = [&](GLint loc, GLfloat v0, GLfloat v1, GLfloat v2,
                       GLfloat v3) {
    uniform4fCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
    EXPECT_EQ(expectedValue.z, v2);
    EXPECT_EQ(expectedValue.w, v3);
  };

  Shader shader = createShader();
  Uniform4f uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform4fCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform1i tests
// ============================================================================

TEST_F(UniformTest, Uniform1i_AssignmentCallsGlUniform1i) {
  GLint location = 7;
  GLint expectedValue = 42;
  bool uniform1iCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform1i = [&](GLint loc, GLint v) {
    uniform1iCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue, v);
  };

  Shader shader = createShader();
  Uniform1i uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform1iCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

TEST_F(UniformTest, Uniform1i_AssignmentSkippedWhenValueUnchanged) {
  int callCount = 0;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return 1; };
  gl.onUniform1i = [&](GLint, GLint) { callCount++; };

  Shader shader = createShader();
  Uniform1i uniform("test", &shader);
  uniform = 10;
  uniform = 10;  // Same value

  EXPECT_EQ(1, callCount);
}

// ============================================================================
// Uniform2i tests
// ============================================================================

TEST_F(UniformTest, Uniform2i_AssignmentCallsGlUniform2i) {
  GLint location = 8;
  glm::ivec2 expectedValue(10, 20);
  bool uniform2iCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform2i = [&](GLint loc, GLint v0, GLint v1) {
    uniform2iCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
  };

  Shader shader = createShader();
  Uniform2i uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform2iCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform3i tests
// ============================================================================

TEST_F(UniformTest, Uniform3i_AssignmentCallsGlUniform3i) {
  GLint location = 9;
  glm::ivec3 expectedValue(10, 20, 30);
  bool uniform3iCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform3i = [&](GLint loc, GLint v0, GLint v1, GLint v2) {
    uniform3iCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
    EXPECT_EQ(expectedValue.z, v2);
  };

  Shader shader = createShader();
  Uniform3i uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform3iCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform4i tests
// ============================================================================

TEST_F(UniformTest, Uniform4i_AssignmentCallsGlUniform4i) {
  GLint location = 10;
  glm::ivec4 expectedValue(10, 20, 30, 40);
  bool uniform4iCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform4i = [&](GLint loc, GLint v0, GLint v1, GLint v2, GLint v3) {
    uniform4iCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
    EXPECT_EQ(expectedValue.z, v2);
    EXPECT_EQ(expectedValue.w, v3);
  };

  Shader shader = createShader();
  Uniform4i uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform4iCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform1ui tests
// ============================================================================

TEST_F(UniformTest, Uniform1ui_AssignmentCallsGlUniform1ui) {
  GLint location = 11;
  GLuint expectedValue = 99;
  bool uniform1uiCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform1ui = [&](GLint loc, GLuint v) {
    uniform1uiCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue, v);
  };

  Shader shader = createShader();
  Uniform1ui uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform1uiCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform2ui tests
// ============================================================================

TEST_F(UniformTest, Uniform2ui_AssignmentCallsGlUniform2ui) {
  GLint location = 12;
  glm::uvec2 expectedValue(100, 200);
  bool uniform2uiCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform2ui = [&](GLint loc, GLuint v0, GLuint v1) {
    uniform2uiCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
  };

  Shader shader = createShader();
  Uniform2ui uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform2uiCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform3ui tests
// ============================================================================

TEST_F(UniformTest, Uniform3ui_AssignmentCallsGlUniform3ui) {
  GLint location = 13;
  glm::uvec3 expectedValue(100, 200, 300);
  bool uniform3uiCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform3ui = [&](GLint loc, GLuint v0, GLuint v1, GLuint v2) {
    uniform3uiCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
    EXPECT_EQ(expectedValue.z, v2);
  };

  Shader shader = createShader();
  Uniform3ui uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform3uiCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform4ui tests
// ============================================================================

TEST_F(UniformTest, Uniform4ui_AssignmentCallsGlUniform4ui) {
  GLint location = 14;
  glm::uvec4 expectedValue(100, 200, 300, 400);
  bool uniform4uiCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform4ui = [&](GLint loc, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    uniform4uiCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedValue.x, v0);
    EXPECT_EQ(expectedValue.y, v1);
    EXPECT_EQ(expectedValue.z, v2);
    EXPECT_EQ(expectedValue.w, v3);
  };

  Shader shader = createShader();
  Uniform4ui uniform("test", &shader);
  uniform = expectedValue;

  EXPECT_TRUE(uniform4uiCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform1fv tests
// ============================================================================

TEST_F(UniformTest, Uniform1fv_AssignmentCallsGlUniform2fv) {
  GLint location = 15;
  GLfloat expectedValue = 2.5f;
  GLsizei expectedCount = 3;
  bool uniform2fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform2fv = [&](GLint loc, GLsizei count, const GLfloat* value) {
    uniform2fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue, *value);
  };

  Shader shader = createShader();
  Uniform1fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform2fvCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform2fv tests
// ============================================================================

TEST_F(UniformTest, Uniform2fv_AssignmentCallsGlUniform2fv) {
  GLint location = 16;
  glm::vec2 expectedValue(1.5f, 2.5f);
  GLsizei expectedCount = 2;
  bool uniform2fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform2fv = [&](GLint loc, GLsizei count, const GLfloat* value) {
    uniform2fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
  };

  Shader shader = createShader();
  Uniform2fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform2fvCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform3fv tests
// ============================================================================

TEST_F(UniformTest, Uniform3fv_AssignmentCallsGlUniform3fv) {
  GLint location = 17;
  glm::vec3 expectedValue(1.5f, 2.5f, 3.5f);
  GLsizei expectedCount = 1;
  bool uniform3fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform3fv = [&](GLint loc, GLsizei count, const GLfloat* value) {
    uniform3fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
    EXPECT_EQ(expectedValue.z, value[2]);
  };

  Shader shader = createShader();
  Uniform3fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform3fvCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform4fv tests
// ============================================================================

TEST_F(UniformTest, Uniform4fv_AssignmentCallsGlUniform4fv) {
  GLint location = 18;
  glm::vec4 expectedValue(1.5f, 2.5f, 3.5f, 4.5f);
  GLsizei expectedCount = 1;
  bool uniform4fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform4fv = [&](GLint loc, GLsizei count, const GLfloat* value) {
    uniform4fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
    EXPECT_EQ(expectedValue.z, value[2]);
    EXPECT_EQ(expectedValue.w, value[3]);
  };

  Shader shader = createShader();
  Uniform4fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform4fvCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform1iv tests
// ============================================================================

TEST_F(UniformTest, Uniform1iv_AssignmentCallsGlUniform1iv) {
  GLint location = 19;
  GLint expectedValue = 42;
  GLsizei expectedCount = 2;
  bool uniform1ivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform1iv = [&](GLint loc, GLsizei count, const GLint* value) {
    uniform1ivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue, *value);
  };

  Shader shader = createShader();
  Uniform1iv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform1ivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform2iv tests
// ============================================================================

TEST_F(UniformTest, Uniform2iv_AssignmentCallsGlUniform2iv) {
  GLint location = 20;
  glm::ivec2 expectedValue(10, 20);
  GLsizei expectedCount = 1;
  bool uniform2ivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform2iv = [&](GLint loc, GLsizei count, const GLint* value) {
    uniform2ivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
  };

  Shader shader = createShader();
  Uniform2iv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform2ivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform3iv tests
// ============================================================================

TEST_F(UniformTest, Uniform3iv_AssignmentCallsGlUniform3iv) {
  GLint location = 21;
  glm::ivec3 expectedValue(10, 20, 30);
  GLsizei expectedCount = 1;
  bool uniform3ivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform3iv = [&](GLint loc, GLsizei count, const GLint* value) {
    uniform3ivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
    EXPECT_EQ(expectedValue.z, value[2]);
  };

  Shader shader = createShader();
  Uniform3iv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform3ivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform4iv tests
// ============================================================================

TEST_F(UniformTest, Uniform4iv_AssignmentCallsGlUniform4iv) {
  GLint location = 22;
  glm::ivec4 expectedValue(10, 20, 30, 40);
  GLsizei expectedCount = 1;
  bool uniform4ivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform4iv = [&](GLint loc, GLsizei count, const GLint* value) {
    uniform4ivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
    EXPECT_EQ(expectedValue.z, value[2]);
    EXPECT_EQ(expectedValue.w, value[3]);
  };

  Shader shader = createShader();
  Uniform4iv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform4ivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform1uiv tests
// ============================================================================

TEST_F(UniformTest, Uniform1uiv_AssignmentCallsGlUniform1uiv) {
  GLint location = 23;
  u32 expectedValue = 99;
  GLsizei expectedCount = 1;
  bool uniform1uivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform1uiv = [&](GLint loc, GLsizei count, const GLuint* value) {
    uniform1uivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue, *value);
  };

  Shader shader = createShader();
  Uniform1uiv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform1uivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform2uiv tests
// ============================================================================

TEST_F(UniformTest, Uniform2uiv_AssignmentCallsGlUniform2uiv) {
  GLint location = 24;
  glm::uvec2 expectedValue(100, 200);
  GLsizei expectedCount = 1;
  bool uniform2uivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform2uiv = [&](GLint loc, GLsizei count, const GLuint* value) {
    uniform2uivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
  };

  Shader shader = createShader();
  Uniform2uiv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform2uivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform3uiv tests
// ============================================================================

TEST_F(UniformTest, Uniform3uiv_AssignmentCallsGlUniform3uiv) {
  GLint location = 25;
  glm::uvec3 expectedValue(100, 200, 300);
  GLsizei expectedCount = 1;
  bool uniform3uivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform3uiv = [&](GLint loc, GLsizei count, const GLuint* value) {
    uniform3uivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
    EXPECT_EQ(expectedValue.z, value[2]);
  };

  Shader shader = createShader();
  Uniform3uiv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform3uivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// Uniform4uiv tests
// ============================================================================

TEST_F(UniformTest, Uniform4uiv_AssignmentCallsGlUniform4uiv) {
  GLint location = 26;
  glm::uvec4 expectedValue(100, 200, 300, 400);
  GLsizei expectedCount = 1;
  bool uniform4uivCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniform4uiv = [&](GLint loc, GLsizei count, const GLuint* value) {
    uniform4uivCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(expectedValue.x, value[0]);
    EXPECT_EQ(expectedValue.y, value[1]);
    EXPECT_EQ(expectedValue.z, value[2]);
    EXPECT_EQ(expectedValue.w, value[3]);
  };

  Shader shader = createShader();
  Uniform4uiv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniform4uivCalled);
  EXPECT_EQ(expectedValue, uniform.value());
}

// ============================================================================
// UniformMatrix2fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix2fv_AssignmentCallsGlUniformMatrix2fv) {
  GLint location = 27;
  glm::mat2 expectedValue(1.0f, 2.0f, 3.0f, 4.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix2fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix2fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                              const GLfloat* value) {
    uniformMatrix2fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix2fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix2fvCalled);
}

TEST_F(UniformTest, UniformMatrix2fv_SkipsWhenLocationInvalid) {
  bool uniformMatrix2fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return -1; };
  gl.onUniformMatrix2fv = [&](GLint, GLsizei, GLboolean, const GLfloat*) {
    uniformMatrix2fvCalled = true;
  };

  Shader shader = createShader();
  UniformMatrix2fv uniform("test", &shader);
  uniform = glm::mat2(1.0f);

  EXPECT_FALSE(uniformMatrix2fvCalled);
}

// ============================================================================
// UniformMatrix3fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix3fv_AssignmentCallsGlUniformMatrix3fv) {
  GLint location = 28;
  glm::mat3 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix3fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix3fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                              const GLfloat* value) {
    uniformMatrix3fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix3fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix3fvCalled);
}

// ============================================================================
// UniformMatrix4fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix4fv_AssignmentCallsGlUniformMatrix4fv) {
  GLint location = 29;
  glm::mat4 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix4fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix4fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                              const GLfloat* value) {
    uniformMatrix4fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix4fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix4fvCalled);
}

// ============================================================================
// UniformMatrix2x3fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix2x3fv_AssignmentCallsGlUniformMatrix2x3fv) {
  GLint location = 30;
  glm::mat2x3 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix2x3fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix2x3fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                                const GLfloat* value) {
    uniformMatrix2x3fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix2x3fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix2x3fvCalled);
}

// ============================================================================
// UniformMatrix3x2fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix3x2fv_AssignmentCallsGlUniformMatrix3x2fv) {
  GLint location = 31;
  glm::mat3x2 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix3x2fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix3x2fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                                const GLfloat* value) {
    uniformMatrix3x2fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix3x2fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix3x2fvCalled);
}

// ============================================================================
// UniformMatrix2x4fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix2x4fv_AssignmentCallsGlUniformMatrix2x4fv) {
  GLint location = 32;
  glm::mat2x4 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix2x4fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix2x4fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                                const GLfloat* value) {
    uniformMatrix2x4fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix2x4fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix2x4fvCalled);
}

// ============================================================================
// UniformMatrix4x2fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix4x2fv_AssignmentCallsGlUniformMatrix4x2fv) {
  GLint location = 33;
  glm::mat4x2 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix4x2fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix4x2fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                                const GLfloat* value) {
    uniformMatrix4x2fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix4x2fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix4x2fvCalled);
}

// ============================================================================
// UniformMatrix3x4fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix3x4fv_AssignmentCallsGlUniformMatrix3x4fv) {
  GLint location = 34;
  glm::mat3x4 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix3x4fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix3x4fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                                const GLfloat* value) {
    uniformMatrix3x4fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix3x4fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix3x4fvCalled);
}

// ============================================================================
// UniformMatrix4x3fv tests
// ============================================================================

TEST_F(UniformTest, UniformMatrix4x3fv_AssignmentCallsGlUniformMatrix4x3fv) {
  GLint location = 35;
  glm::mat4x3 expectedValue(1.0f);
  GLsizei expectedCount = 1;
  bool uniformMatrix4x3fvCalled = false;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [&](GLuint, const GLchar*) -> GLint {
    return location;
  };
  gl.onUniformMatrix4x3fv = [&](GLint loc, GLsizei count, GLboolean transpose,
                                const GLfloat* value) {
    uniformMatrix4x3fvCalled = true;
    EXPECT_EQ(location, loc);
    EXPECT_EQ(expectedCount, count);
    EXPECT_EQ(GL_FALSE, transpose);
    EXPECT_NE(nullptr, value);
  };

  Shader shader = createShader();
  UniformMatrix4x3fv uniform("test", &shader, expectedCount);
  uniform = expectedValue;

  EXPECT_TRUE(uniformMatrix4x3fvCalled);
}

// ============================================================================
// Matrix uniforms do not check for value change (no caching)
// ============================================================================

TEST_F(UniformTest, UniformMatrix4fv_AlwaysUpdatesEvenWithSameValue) {
  int callCount = 0;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return 1; };
  gl.onUniformMatrix4fv = [&](GLint, GLsizei, GLboolean, const GLfloat*) {
    callCount++;
  };

  Shader shader = createShader();
  UniformMatrix4fv uniform("test", &shader);
  glm::mat4 value(1.0f);
  uniform = value;
  uniform = value;  // Same value - should still call GL (no value caching)

  EXPECT_EQ(2, callCount);
}

// ============================================================================
// Default count parameter tests
// ============================================================================

TEST_F(UniformTest, UniformVectorTypes_DefaultCountIsOne) {
  GLsizei capturedCount = 0;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return 1; };
  gl.onUniform2fv = [&](GLint, GLsizei count, const GLfloat*) {
    capturedCount = count;
  };

  Shader shader = createShader();
  // No count parameter - should default to 1
  Uniform2fv uniform("test", &shader);
  uniform = glm::vec2(1.0f, 2.0f);

  EXPECT_EQ(1, capturedCount);
}

TEST_F(UniformTest, UniformMatrixTypes_DefaultCountIsOne) {
  GLsizei capturedCount = 0;

  gl.onCreateProgram = []() -> GLuint { return 1; };
  gl.onGetUniformLocation = [](GLuint, const GLchar*) -> GLint { return 1; };
  gl.onUniformMatrix4fv = [&](GLint, GLsizei count, GLboolean, const GLfloat*) {
    capturedCount = count;
  };

  Shader shader = createShader();
  // No count parameter - should default to 1
  UniformMatrix4fv uniform("test", &shader);
  uniform = glm::mat4(1.0f);

  EXPECT_EQ(1, capturedCount);
}

}  // namespace uinta
