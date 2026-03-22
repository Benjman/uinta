#include "uinta/camera/camera.h"

#include <gtest/gtest.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/epsilon.hpp>

#include "uinta/camera/camera_input_deltas.h"
#include "uinta/math/defs.h"

namespace uinta {

class CameraTest : public ::testing::Test {
 protected:
  static CameraConfig makeConfig() noexcept {
    CameraConfig config;
    config.dstMin = 1.0f;
    config.dstMax = 100.0f;
    config.pitchMin = -89.0f;
    config.pitchMax = 89.0f;
    config.trnslScaleFct = 1.0f;
    config.spdFactorMin = 0.1f;
    config.vertOff = 0.0f;
    config.angle = SmoothFloat(10.0f, 0.0f);
    config.dist = SmoothFloat(10.0f, 10.0f);
    config.pitch = SmoothFloat(10.0f, 30.0f);
    config.target = SmoothVec3(10.0f, 0.0f, 0.0f, 0.0f);
    return config;
  }
};

// Constructor tests
TEST_F(CameraTest, ConstructorWithConfig) {
  auto config = makeConfig();
  Camera camera(config);
  EXPECT_FLOAT_EQ(config.angle.current(), camera.angle().current());
  EXPECT_FLOAT_EQ(config.dist.current(), camera.dist().current());
  EXPECT_FLOAT_EQ(config.pitch.current(), camera.pitch().current());
}

TEST_F(CameraTest, ConstructorInitializesPositionToZero) {
  Camera camera(makeConfig());
  auto pos = camera.position();
  EXPECT_FLOAT_EQ(0.0f, pos.x);
  EXPECT_FLOAT_EQ(0.0f, pos.y);
  EXPECT_FLOAT_EQ(0.0f, pos.z);
}

// Accessor tests
TEST_F(CameraTest, AngleAccessor) {
  auto config = makeConfig();
  config.angle = SmoothFloat(5.0f, 45.0f);
  Camera camera(config);
  EXPECT_FLOAT_EQ(45.0f, camera.angle().current());
}

TEST_F(CameraTest, AngleMutatorFloat) {
  Camera camera(makeConfig());
  camera.angle(90.0f);
  EXPECT_FLOAT_EQ(90.0f, camera.angle().target());
}

TEST_F(CameraTest, AngleMutatorSmoothFloat) {
  Camera camera(makeConfig());
  SmoothFloat newAngle(5.0f, 120.0f);
  camera.angle(newAngle);
  EXPECT_FLOAT_EQ(120.0f, camera.angle().current());
}

TEST_F(CameraTest, DistAccessor) {
  auto config = makeConfig();
  config.dist = SmoothFloat(5.0f, 25.0f);
  Camera camera(config);
  EXPECT_FLOAT_EQ(25.0f, camera.dist().current());
}

TEST_F(CameraTest, DistMutatorFloat) {
  Camera camera(makeConfig());
  camera.dist(50.0f);
  EXPECT_FLOAT_EQ(50.0f, camera.dist().target());
}

TEST_F(CameraTest, DistMutatorSmoothFloat) {
  Camera camera(makeConfig());
  SmoothFloat newDist(5.0f, 75.0f);
  camera.dist(newDist);
  EXPECT_FLOAT_EQ(75.0f, camera.dist().current());
}

TEST_F(CameraTest, PitchAccessor) {
  auto config = makeConfig();
  config.pitch = SmoothFloat(5.0f, 60.0f);
  Camera camera(config);
  EXPECT_FLOAT_EQ(60.0f, camera.pitch().current());
}

TEST_F(CameraTest, PitchMutatorFloat) {
  Camera camera(makeConfig());
  camera.pitch(45.0f);
  EXPECT_FLOAT_EQ(45.0f, camera.pitch().target());
}

TEST_F(CameraTest, PitchMutatorSmoothFloat) {
  Camera camera(makeConfig());
  SmoothFloat newPitch(5.0f, 30.0f);
  camera.pitch(newPitch);
  EXPECT_FLOAT_EQ(30.0f, camera.pitch().current());
}

TEST_F(CameraTest, TargetAccessor) {
  auto config = makeConfig();
  config.target = SmoothVec3(5.0f, 1.0f, 2.0f, 3.0f);
  Camera camera(config);
  auto target = static_cast<glm::vec3>(camera.target());
  EXPECT_FLOAT_EQ(1.0f, target.x);
  EXPECT_FLOAT_EQ(2.0f, target.y);
  EXPECT_FLOAT_EQ(3.0f, target.z);
}

TEST_F(CameraTest, TargetMutatorSmoothVec3) {
  Camera camera(makeConfig());
  SmoothVec3 newTarget(5.0f, 5.0f, 6.0f, 7.0f);
  camera.target(newTarget);
  auto target = static_cast<glm::vec3>(camera.target());
  EXPECT_FLOAT_EQ(5.0f, target.x);
  EXPECT_FLOAT_EQ(6.0f, target.y);
  EXPECT_FLOAT_EQ(7.0f, target.z);
}

TEST_F(CameraTest, TargetMutatorGlmVec3) {
  Camera camera(makeConfig());
  camera.target(glm::vec3(10.0f, 11.0f, 12.0f));
  EXPECT_FLOAT_EQ(10.0f, camera.target().x().target());
  EXPECT_FLOAT_EQ(11.0f, camera.target().y().target());
  EXPECT_FLOAT_EQ(12.0f, camera.target().z().target());
}

TEST_F(CameraTest, FlagsAccessorConst) {
  auto config = makeConfig();
  config.flags.isKeyboardEnabled(true);
  const Camera camera(config);
  EXPECT_TRUE(camera.flags().isKeyboardEnabled());
}

TEST_F(CameraTest, FlagsAccessorNonConst) {
  Camera camera(makeConfig());
  camera.flags().isMouseEnabled(false);
  EXPECT_FALSE(camera.flags().isMouseEnabled());
}

TEST_F(CameraTest, FlagsMutator) {
  Camera camera(makeConfig());
  CameraConfig::Flags newFlags;
  newFlags.isKeyboardEnabled(false);
  camera.flags(newFlags);
  EXPECT_FALSE(camera.flags().isKeyboardEnabled());
}

// Enable/Disable tests
TEST_F(CameraTest, EnableKeyboard) {
  auto config = makeConfig();
  config.flags.isKeyboardEnabled(false);
  Camera camera(config);
  EXPECT_FALSE(camera.flags().isKeyboardEnabled());
  camera.enableKeyboard();
  EXPECT_TRUE(camera.flags().isKeyboardEnabled());
}

TEST_F(CameraTest, DisableKeyboard) {
  auto config = makeConfig();
  config.flags.isKeyboardEnabled(true);
  Camera camera(config);
  camera.enableKeyboard(false);
  EXPECT_FALSE(camera.flags().isKeyboardEnabled());
}

TEST_F(CameraTest, EnableMouse) {
  auto config = makeConfig();
  config.flags.isMouseEnabled(false);
  Camera camera(config);
  EXPECT_FALSE(camera.flags().isMouseEnabled());
  camera.enableMouse();
  EXPECT_TRUE(camera.flags().isMouseEnabled());
}

TEST_F(CameraTest, DisableMouse) {
  auto config = makeConfig();
  config.flags.isMouseEnabled(true);
  Camera camera(config);
  camera.enableMouse(false);
  EXPECT_FALSE(camera.flags().isMouseEnabled());
}

// Lock tests
TEST_F(CameraTest, LockAngle) {
  Camera camera(makeConfig());
  EXPECT_FALSE(camera.flags().isAngleLock());
  camera.lockAngle();
  EXPECT_TRUE(camera.flags().isAngleLock());
}

TEST_F(CameraTest, UnlockAngle) {
  auto config = makeConfig();
  config.flags.isAngleLock(true);
  Camera camera(config);
  camera.lockAngle(false);
  EXPECT_FALSE(camera.flags().isAngleLock());
}

TEST_F(CameraTest, LockDist) {
  Camera camera(makeConfig());
  EXPECT_FALSE(camera.flags().isDistLock());
  camera.lockDist();
  EXPECT_TRUE(camera.flags().isDistLock());
}

TEST_F(CameraTest, UnlockDist) {
  auto config = makeConfig();
  config.flags.isDistLock(true);
  Camera camera(config);
  camera.lockDist(false);
  EXPECT_FALSE(camera.flags().isDistLock());
}

TEST_F(CameraTest, LockPitch) {
  Camera camera(makeConfig());
  EXPECT_FALSE(camera.flags().isPitchLock());
  camera.lockPitch();
  EXPECT_TRUE(camera.flags().isPitchLock());
}

TEST_F(CameraTest, UnlockPitch) {
  auto config = makeConfig();
  config.flags.isPitchLock(true);
  Camera camera(config);
  camera.lockPitch(false);
  EXPECT_FALSE(camera.flags().isPitchLock());
}

// Force tests
TEST_F(CameraTest, ForceUpdatesAllSmoothValues) {
  Camera camera(makeConfig());
  camera.angle(90.0f);
  camera.dist(50.0f);
  camera.pitch(45.0f);
  camera.target(glm::vec3(1.0f, 2.0f, 3.0f));

  // Before force, current values should differ from targets
  camera.force();

  // After force, current should equal target
  EXPECT_FLOAT_EQ(90.0f, camera.angle().current());
  EXPECT_FLOAT_EQ(50.0f, camera.dist().current());
  EXPECT_FLOAT_EQ(45.0f, camera.pitch().current());
  auto target = static_cast<glm::vec3>(camera.target());
  EXPECT_FLOAT_EQ(1.0f, target.x);
  EXPECT_FLOAT_EQ(2.0f, target.y);
  EXPECT_FLOAT_EQ(3.0f, target.z);
}

// TranslationFactor tests
TEST_F(CameraTest, TranslationFactorBasic) {
  auto config = makeConfig();
  config.dist = SmoothFloat(10.0f, 10.0f);
  config.trnslScaleFct = 1.0f;
  config.spdFactorMin = 0.1f;
  Camera camera(config);
  camera.force();

  f32 factor = camera.translationFactor();
  // Formula: max(spdFactorMin, 1.3 * sqrt(abs(dist)) * trnslScaleFct)
  // = max(0.1, 1.3 * sqrt(10) * 1.0) = 1.3 * ~3.16 = ~4.11
  EXPECT_GT(factor, 4.0f);
  EXPECT_LT(factor, 4.2f);
}

TEST_F(CameraTest, TranslationFactorRespectsMinimum) {
  auto config = makeConfig();
  config.dist = SmoothFloat(10.0f, 0.0f);
  config.trnslScaleFct = 0.001f;
  config.spdFactorMin = 1.0f;
  Camera camera(config);
  camera.force();

  f32 factor = camera.translationFactor();
  EXPECT_FLOAT_EQ(1.0f, factor);  // Should be clamped to minimum
}

// Update tests
TEST_F(CameraTest, UpdateComputesPosition) {
  auto config = makeConfig();
  config.angle = SmoothFloat(1000.0f, 0.0f);
  config.pitch = SmoothFloat(1000.0f, 0.0f);
  config.dist = SmoothFloat(1000.0f, 10.0f);
  config.target = SmoothVec3(1000.0f, 0.0f, 0.0f, 0.0f);
  Camera camera(config);
  camera.force();
  camera.update(0.016, CameraInputDeltas{});

  // With angle=0, pitch=0, dist=10, target=(0,0,0)
  // Position should be approximately (0, 0, 10)
  auto pos = camera.position();
  EXPECT_NEAR(0.0f, pos.x, 0.01f);
  EXPECT_NEAR(0.0f, pos.y, 0.01f);
  EXPECT_NEAR(10.0f, pos.z, 0.01f);
}

TEST_F(CameraTest, UpdateWithAngle) {
  auto config = makeConfig();
  config.angle = SmoothFloat(1000.0f, 90.0f);
  config.pitch = SmoothFloat(1000.0f, 0.0f);
  config.dist = SmoothFloat(1000.0f, 10.0f);
  config.target = SmoothVec3(1000.0f, 0.0f, 0.0f, 0.0f);
  Camera camera(config);
  camera.force();
  camera.update(0.016, CameraInputDeltas{});

  // With angle=90, camera should be rotated
  auto pos = camera.position();
  EXPECT_NEAR(-10.0f, pos.x, 0.01f);
  EXPECT_NEAR(0.0f, pos.y, 0.01f);
  EXPECT_NEAR(0.0f, pos.z, 0.01f);
}

TEST_F(CameraTest, UpdateWithPitch) {
  auto config = makeConfig();
  config.angle = SmoothFloat(1000.0f, 0.0f);
  config.pitch = SmoothFloat(1000.0f, 45.0f);
  config.dist = SmoothFloat(1000.0f, 10.0f);
  config.target = SmoothVec3(1000.0f, 0.0f, 0.0f, 0.0f);
  Camera camera(config);
  camera.force();
  camera.update(0.016, CameraInputDeltas{});

  auto pos = camera.position();
  // With pitch=45 degrees, y component should be ~7.07 (10 * sin(45))
  EXPECT_NEAR(0.0f, pos.x, 0.01f);
  EXPECT_GT(pos.y, 7.0f);
  EXPECT_LT(pos.y, 7.1f);
}

TEST_F(CameraTest, UpdateWithTarget) {
  auto config = makeConfig();
  config.angle = SmoothFloat(1000.0f, 0.0f);
  config.pitch = SmoothFloat(1000.0f, 0.0f);
  config.dist = SmoothFloat(1000.0f, 10.0f);
  config.target = SmoothVec3(1000.0f, 5.0f, 0.0f, 5.0f);
  Camera camera(config);
  camera.force();
  camera.update(0.016, CameraInputDeltas{});

  auto pos = camera.position();
  // Position should be target + distance offset
  EXPECT_NEAR(5.0f, pos.x, 0.01f);
  EXPECT_NEAR(0.0f, pos.y, 0.01f);
  EXPECT_NEAR(15.0f, pos.z, 0.01f);
}

TEST_F(CameraTest, UpdateRespectsAngleLock) {
  auto config = makeConfig();
  config.angle = SmoothFloat(1000.0f, 45.0f);
  config.flags.isAngleLock(true);
  Camera camera(config);
  camera.force();

  // Try to apply angle delta when angle is locked
  CameraInputDeltas deltas{};
  deltas.angle = 30.0f;
  camera.update(1.0, deltas);

  // Angle should remain unchanged due to lock
  EXPECT_FLOAT_EQ(45.0f, camera.angle().current());
}

TEST_F(CameraTest, UpdateRespectsPitchLock) {
  auto config = makeConfig();
  config.pitch = SmoothFloat(1000.0f, 30.0f);
  config.flags.isPitchLock(true);
  Camera camera(config);
  camera.force();

  // Try to apply pitch delta when pitch is locked
  CameraInputDeltas deltas{};
  deltas.pitch = 20.0f;
  camera.update(1.0, deltas);

  // Pitch should remain unchanged due to lock
  EXPECT_FLOAT_EQ(30.0f, camera.pitch().current());
}

TEST_F(CameraTest, UpdateRespectsDistLock) {
  auto config = makeConfig();
  config.dist = SmoothFloat(1000.0f, 20.0f);
  config.flags.isDistLock(true);
  Camera camera(config);
  camera.force();

  // Try to apply dist delta when dist is locked
  CameraInputDeltas deltas{};
  deltas.dist = 10.0f;
  camera.update(1.0, deltas);

  // Distance should remain unchanged due to lock
  EXPECT_FLOAT_EQ(20.0f, camera.dist().current());
}

// ViewMatrix tests
TEST_F(CameraTest, ViewMatrixIsValid) {
  Camera camera(makeConfig());
  camera.force();
  camera.update(0.016, CameraInputDeltas{});

  glm::mat4 view = camera.viewMatrix();

  // View matrix should not be identity or zero
  EXPECT_FALSE(glm::all(glm::epsilonEqual(view[0], glm::vec4(0), epsilon_f)));
  EXPECT_NE(view[3][3], 0.0f);
}

}  // namespace uinta
