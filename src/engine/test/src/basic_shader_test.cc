#include "uinta/shaders/basic_shader.h"

#include <gtest/gtest.h>

#include <vector>

#include "./utils.h"
#include "uinta/mock/mock_platform.h"
#include "uinta/mock/mock_scene.h"

namespace uinta {
namespace {

// BasicShader tests
class BasicShaderTest : public UintaTestF {};

TEST_F(BasicShaderTest, ConstructorCreatesProgramId) {
  GLuint expectedProgramId = 42;
  gl.onCreateProgram = [&]() -> GLuint { return expectedProgramId; };

  BasicShader shader(&gl);

  EXPECT_EQ(expectedProgramId, shader.id());
}

TEST_F(BasicShaderTest, ConstructorStoresGlPointer) {
  BasicShader shader(&gl);

  EXPECT_EQ(&gl, shader.gl());
}

TEST_F(BasicShaderTest, ConstructorLinksProgram) {
  bool linkProgramCalled = false;
  GLuint linkedProgramId = 0;
  gl.onLinkProgram = [&](GLuint program) {
    linkProgramCalled = true;
    linkedProgramId = program;
  };

  GLuint expectedProgramId = 99;
  gl.onCreateProgram = [&]() -> GLuint { return expectedProgramId; };

  BasicShader shader(&gl);

  EXPECT_TRUE(linkProgramCalled);
  EXPECT_EQ(expectedProgramId, linkedProgramId);
}

TEST_F(BasicShaderTest, ConstructorInitializesUniforms) {
  std::vector<std::pair<GLint, GLfloat>> uniform1fCalls;
  std::vector<std::pair<GLint, GLint>> uniform1iCalls;
  std::vector<std::tuple<GLint, GLfloat, GLfloat, GLfloat>> uniform3fCalls;
  std::vector<GLint> uniformMatrix4fvCalls;

  gl.onUniform1f = [&](GLint location, GLfloat value) {
    uniform1fCalls.emplace_back(location, value);
  };
  gl.onUniform1i = [&](GLint location, GLint value) {
    uniform1iCalls.emplace_back(location, value);
  };
  gl.onUniform3f = [&](GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    uniform3fCalls.emplace_back(location, v0, v1, v2);
  };
  gl.onUniformMatrix4fv = [&](GLint location, GLsizei, GLboolean,
                              const GLfloat*) {
    uniformMatrix4fvCalls.push_back(location);
  };

  BasicShader shader(&gl);

  // ambientStr=0.4 and diffuseMin=0.125 trigger calls; time=0 is a no-op
  EXPECT_EQ(uniform1fCalls.size(), 2u);

  // flags=0 is a no-op (default-initialized value already 0)
  EXPECT_EQ(uniform1iCalls.size(), 0u);

  // lightColor and lightDir trigger calls (non-zero values)
  EXPECT_EQ(uniform3fCalls.size(), 2u);

  // model, projection, and view always trigger calls
  EXPECT_EQ(uniformMatrix4fvCalls.size(), 3u);
}

TEST_F(BasicShaderTest, DestructorDeletesProgram) {
  bool deleteProgramCalled = false;
  GLuint deletedProgramId = 0;
  gl.onDeleteProgram = [&](GLuint program) {
    deleteProgramCalled = true;
    deletedProgramId = program;
  };

  GLuint expectedProgramId = 77;
  gl.onCreateProgram = [&]() -> GLuint { return expectedProgramId; };

  {
    BasicShader shader(&gl);
  }

  EXPECT_TRUE(deleteProgramCalled);
  EXPECT_EQ(expectedProgramId, deletedProgramId);
}

TEST_F(BasicShaderTest, LinkAttributesSetsUpFourAttributes) {
  std::vector<GLuint> enabledAttributes;
  std::vector<GLuint> vertexAttribPointerIndices;

  gl.onEnableVertexAttribArray = [&](GLuint index) {
    enabledAttributes.push_back(index);
  };
  gl.onVertexAttribPointer = [&](GLuint index, GLint, GLenum, GLboolean,
                                 GLsizei, const void*) {
    vertexAttribPointerIndices.push_back(index);
  };

  BasicShader shader(&gl);

  // Clear the vectors since Vao constructor may also call these
  enabledAttributes.clear();
  vertexAttribPointerIndices.clear();

  // Create a minimal Vao to test linkAttributes
  Vao vao(&gl);
  shader.linkAttributes(&vao);

  // Should have 4 attributes: position(0), normal(1), color(2), uv(3)
  EXPECT_EQ(4u, enabledAttributes.size());
  EXPECT_EQ(4u, vertexAttribPointerIndices.size());

  // Verify attribute indices
  EXPECT_NE(
      std::ranges::find(enabledAttributes.begin(), enabledAttributes.end(), 0u),
      enabledAttributes.end());
  EXPECT_NE(
      std::ranges::find(enabledAttributes.begin(), enabledAttributes.end(), 1u),
      enabledAttributes.end());
  EXPECT_NE(
      std::ranges::find(enabledAttributes.begin(), enabledAttributes.end(), 2u),
      enabledAttributes.end());
  EXPECT_NE(
      std::ranges::find(enabledAttributes.begin(), enabledAttributes.end(), 3u),
      enabledAttributes.end());
}

// BasicShaderManager::Flags tests
class BasicShaderManagerFlagsTest : public ::testing::Test {};

TEST_F(BasicShaderManagerFlagsTest, DefaultFlagsAreDirty) {
  BasicShaderManager::Flags flags;

  EXPECT_TRUE(flags.isViewDirty());
  EXPECT_TRUE(flags.isProjectionDirty());
}

TEST_F(BasicShaderManagerFlagsTest, ViewDirtyCanBeCleared) {
  BasicShaderManager::Flags flags;

  flags.isViewDirty(false);
  EXPECT_FALSE(flags.isViewDirty());
  EXPECT_TRUE(flags.isProjectionDirty());
}

TEST_F(BasicShaderManagerFlagsTest, ProjectionDirtyCanBeCleared) {
  BasicShaderManager::Flags flags;

  flags.isProjectionDirty(false);
  EXPECT_TRUE(flags.isViewDirty());
  EXPECT_FALSE(flags.isProjectionDirty());
}

TEST_F(BasicShaderManagerFlagsTest, ViewDirtyCanBeSetAfterClearing) {
  BasicShaderManager::Flags flags;

  flags.isViewDirty(false);
  EXPECT_FALSE(flags.isViewDirty());

  flags.isViewDirty(true);
  EXPECT_TRUE(flags.isViewDirty());
}

TEST_F(BasicShaderManagerFlagsTest, ProjectionDirtyCanBeSetAfterClearing) {
  BasicShaderManager::Flags flags;

  flags.isProjectionDirty(false);
  EXPECT_FALSE(flags.isProjectionDirty());

  flags.isProjectionDirty(true);
  EXPECT_TRUE(flags.isProjectionDirty());
}

TEST_F(BasicShaderManagerFlagsTest, FlagsAreIndependent) {
  BasicShaderManager::Flags flags;

  flags.isViewDirty(false);
  flags.isProjectionDirty(true);
  EXPECT_FALSE(flags.isViewDirty());
  EXPECT_TRUE(flags.isProjectionDirty());

  flags.isViewDirty(true);
  flags.isProjectionDirty(false);
  EXPECT_TRUE(flags.isViewDirty());
  EXPECT_FALSE(flags.isProjectionDirty());
}

// BasicShaderManager tests
class BasicShaderManagerTest : public UintaTestF {};

TEST_F(BasicShaderManagerTest, ConstructorCreatesShaderFromScene) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);

  EXPECT_NE(nullptr, manager.shader());
}

TEST_F(BasicShaderManagerTest, ShaderAccessorReturnsValidPointer) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);

  BasicShader* shader = manager.shader();
  const BasicShader* constShader =
      const_cast<const BasicShaderManager&>(manager).shader();

  EXPECT_EQ(shader, constShader);
}

TEST_F(BasicShaderManagerTest, RenderCountInitializedToZero) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);

  EXPECT_EQ(0u, manager.renderCount());
}

TEST_F(BasicShaderManagerTest, RenderCountCanBeSet) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.renderCount(42);

  EXPECT_EQ(42u, manager.renderCount());
}

TEST_F(BasicShaderManagerTest, UpdateResetsRenderCount) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.renderCount(100);

  manager.update(0.016);

  EXPECT_EQ(0u, manager.renderCount());
}

TEST_F(BasicShaderManagerTest, UpdateAccumulatesRuntime) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);

  manager.update(0.5);
  EXPECT_FLOAT_EQ(0.5f, manager.time());

  manager.update(0.3);
  EXPECT_FLOAT_EQ(0.8f, manager.time());
}

// DrawElements primitive counting tests
class DrawElementsCountingTest : public UintaTestF {};

TEST_F(DrawElementsCountingTest, TrianglesCountCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(3u, manager.renderCount());  // 9 / 3 = 3 triangles
}

TEST_F(DrawElementsCountingTest, TriangleStripCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(3u, manager.renderCount());  // 5 - 2 = 3 triangles
}

TEST_F(DrawElementsCountingTest, TriangleFanCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(4u, manager.renderCount());  // 6 - 2 = 4 triangles
}

TEST_F(DrawElementsCountingTest, LinesCountCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(4u, manager.renderCount());  // 8 / 2 = 4 lines
}

TEST_F(DrawElementsCountingTest, LineStripCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(4u, manager.renderCount());  // 5 - 1 = 4 lines
}

TEST_F(DrawElementsCountingTest, LineLoopCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(4u, manager.renderCount());  // 4 lines (loop back to start)
}

TEST_F(DrawElementsCountingTest, PointsCountCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_POINTS, 10, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(10u, manager.renderCount());  // 10 points
}

TEST_F(DrawElementsCountingTest, TrianglesAdjacencyCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_TRIANGLES_ADJACENCY, 12, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(2u, manager.renderCount());  // 12 / 6 = 2 triangles
}

TEST_F(DrawElementsCountingTest, TriangleStripAdjacencyCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_TRIANGLE_STRIP_ADJACENCY, 10, GL_UNSIGNED_INT,
                       nullptr);

  EXPECT_EQ(6u, manager.renderCount());  // 10 - 4 = 6 triangles
}

TEST_F(DrawElementsCountingTest, LinesAdjacencyCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_LINES_ADJACENCY, 8, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(2u, manager.renderCount());  // 8 / 4 = 2 lines
}

TEST_F(DrawElementsCountingTest, LineStripAdjacencyCountsCorrectly) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_LINE_STRIP_ADJACENCY, 7, GL_UNSIGNED_INT, nullptr);

  EXPECT_EQ(4u, manager.renderCount());  // 7 - 3 = 4 lines
}

TEST_F(DrawElementsCountingTest, MultipleDrawCallsAccumulate) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // 2
  manager.drawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);  // 3

  EXPECT_EQ(5u, manager.renderCount());  // 2 + 3 = 5 triangles
}

// DrawArrays tests
class DrawArraysTest : public UintaTestF {};

TEST_F(DrawArraysTest, DrawArraysCallsGlDrawArrays) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  bool drawArraysCalled = false;
  GLenum capturedMode = 0;
  GLint capturedFirst = 0;
  GLsizei capturedCount = 0;

  gl.onDrawArrays = [&](GLenum mode, GLint first, GLsizei count) {
    drawArraysCalled = true;
    capturedMode = mode;
    capturedFirst = first;
    capturedCount = count;
  };

  BasicShaderManager manager(&engine);
  manager.drawArrays(GL_TRIANGLES, 0, 12);

  EXPECT_TRUE(drawArraysCalled);
  EXPECT_EQ(GL_TRIANGLES, capturedMode);
  EXPECT_EQ(0, capturedFirst);
  EXPECT_EQ(12, capturedCount);
}

TEST_F(DrawArraysTest, DrawArraysUpdatesRenderCount) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.drawArrays(GL_TRIANGLES, 0, 15);

  EXPECT_EQ(15u, manager.renderCount());
}

// Uniform accessors tests
class UniformAccessorsTest : public UintaTestF {};

TEST_F(UniformAccessorsTest, AmbientStrGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.ambientStr(0.75f);

  EXPECT_FLOAT_EQ(0.75f, manager.ambientStr());
}

TEST_F(UniformAccessorsTest, DiffuseMinGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.diffuseMin(0.25f);

  EXPECT_FLOAT_EQ(0.25f, manager.diffuseMin());
}

TEST_F(UniformAccessorsTest, TimeGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.time(1.5f);

  EXPECT_FLOAT_EQ(1.5f, manager.time());
}

TEST_F(UniformAccessorsTest, FlagsGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.flags(7.0f);

  EXPECT_FLOAT_EQ(7.0f, manager.flags());
}

TEST_F(UniformAccessorsTest, LightColorGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  glm::vec3 color(0.5f, 0.6f, 0.7f);
  manager.lightColor(color);

  glm::vec3 result = manager.lightColor();
  EXPECT_FLOAT_EQ(0.5f, result.x);
  EXPECT_FLOAT_EQ(0.6f, result.y);
  EXPECT_FLOAT_EQ(0.7f, result.z);
}

TEST_F(UniformAccessorsTest, LightDirGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  glm::vec3 dir(0.1f, -0.9f, 0.2f);
  manager.lightDir(dir);

  glm::vec3 result = manager.lightDir();
  EXPECT_FLOAT_EQ(0.1f, result.x);
  EXPECT_FLOAT_EQ(-0.9f, result.y);
  EXPECT_FLOAT_EQ(0.2f, result.z);
}

TEST_F(UniformAccessorsTest, ModelGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  auto model = glm::mat4(2.0f);
  manager.model(model);

  glm::mat4 result = manager.model();
  EXPECT_FLOAT_EQ(2.0f, result[0][0]);
}

TEST_F(UniformAccessorsTest, ProjectionGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  auto projection = glm::mat4(3.0f);
  manager.projection(projection);

  glm::mat4 result = manager.projection();
  EXPECT_FLOAT_EQ(3.0f, result[0][0]);
}

TEST_F(UniformAccessorsTest, ViewGetterSetter) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  auto view = glm::mat4(4.0f);
  manager.view(view);

  glm::mat4 result = manager.view();
  EXPECT_FLOAT_EQ(4.0f, result[0][0]);
}

// Flag mask methods tests
class FlagMaskMethodsTest : public UintaTestF {};

TEST_F(FlagMaskMethodsTest, SwayEnablesSwayFlag) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.sway(true);

  EXPECT_EQ(BasicShaderManager::Masks::Sway,
            static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Sway);
}

TEST_F(FlagMaskMethodsTest, SwayDisablesSwayFlag) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.sway(true);
  manager.sway(false);

  EXPECT_EQ(0,
            static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Sway);
}

TEST_F(FlagMaskMethodsTest, UvEnablesUvFlag) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.uv(true);

  EXPECT_EQ(BasicShaderManager::Masks::Uv,
            static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Uv);
}

TEST_F(FlagMaskMethodsTest, UvDisablesUvFlag) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.uv(true);
  manager.uv(false);

  EXPECT_EQ(0,
            static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Uv);
}

TEST_F(FlagMaskMethodsTest, NormalsEnablesNormalsFlag) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.normals(true);

  EXPECT_EQ(
      BasicShaderManager::Masks::Normals,
      static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Normals);
}

TEST_F(FlagMaskMethodsTest, NormalsDisablesNormalsFlag) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.normals(true);
  manager.normals(false);

  EXPECT_EQ(
      0, static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Normals);
}

TEST_F(FlagMaskMethodsTest, MultipleFlagsCanBeEnabled) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.sway(true);
  manager.uv(true);
  manager.normals(true);

  u8 flags = static_cast<u8>(manager.flags());
  EXPECT_EQ(BasicShaderManager::Masks::Sway,
            flags & BasicShaderManager::Masks::Sway);
  EXPECT_EQ(BasicShaderManager::Masks::Uv,
            flags & BasicShaderManager::Masks::Uv);
  EXPECT_EQ(BasicShaderManager::Masks::Normals,
            flags & BasicShaderManager::Masks::Normals);
}

TEST_F(FlagMaskMethodsTest, EnablingAlreadyEnabledFlagIsNoOp) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.sway(true);
  manager.sway(true);

  EXPECT_EQ(BasicShaderManager::Masks::Sway,
            static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Sway);
}

TEST_F(FlagMaskMethodsTest, DisablingAlreadyDisabledFlagIsNoOp) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  BasicShaderManager manager(&engine);
  manager.sway(false);

  EXPECT_EQ(0,
            static_cast<u8>(manager.flags()) & BasicShaderManager::Masks::Sway);
}

// LinkAttributes delegation test
class LinkAttributesDelegationTest : public UintaTestF {};

TEST_F(LinkAttributesDelegationTest, ManagerDelegatesToShader) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);
  MockScene scene(&engine);

  std::vector<GLuint> enabledAttributes;
  gl.onEnableVertexAttribArray = [&](GLuint index) {
    enabledAttributes.push_back(index);
  };

  BasicShaderManager manager(&engine);

  enabledAttributes.clear();

  Vao vao(&gl);
  manager.linkAttributes(&vao);

  EXPECT_EQ(4u, enabledAttributes.size());
}

}  // namespace
}  // namespace uinta
