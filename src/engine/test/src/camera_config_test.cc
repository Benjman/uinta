#include "uinta/camera/camera_config.h"

#include <gtest/gtest.h>

#include "uinta/camera/camera_keybindings.h"

namespace uinta {
namespace {

// Flags tests
class CameraConfigFlagsTest : public ::testing::Test {
 protected:
  CameraConfig::Flags flags_;
};

TEST_F(CameraConfigFlagsTest, DefaultFlagsHaveExpectedValues) {
  // Default flags should have DistLimit, PitchLimit, Keyboard, Mouse enabled
  EXPECT_TRUE(flags_.isDistLimit());
  EXPECT_TRUE(flags_.isPitchLimit());
  EXPECT_TRUE(flags_.isKeyboardEnabled());
  EXPECT_TRUE(flags_.isMouseEnabled());
  EXPECT_FALSE(flags_.isAngleLock());
  EXPECT_FALSE(flags_.isDistLock());
  EXPECT_FALSE(flags_.isPitchLock());
}

TEST_F(CameraConfigFlagsTest, IsAngleLockSetGet) {
  EXPECT_FALSE(flags_.isAngleLock());
  flags_.isAngleLock(true);
  EXPECT_TRUE(flags_.isAngleLock());
  flags_.isAngleLock(false);
  EXPECT_FALSE(flags_.isAngleLock());
}

TEST_F(CameraConfigFlagsTest, IsDistLimitSetGet) {
  EXPECT_TRUE(flags_.isDistLimit());
  flags_.isDistLimit(false);
  EXPECT_FALSE(flags_.isDistLimit());
  flags_.isDistLimit(true);
  EXPECT_TRUE(flags_.isDistLimit());
}

TEST_F(CameraConfigFlagsTest, IsDistLockSetGet) {
  EXPECT_FALSE(flags_.isDistLock());
  flags_.isDistLock(true);
  EXPECT_TRUE(flags_.isDistLock());
  flags_.isDistLock(false);
  EXPECT_FALSE(flags_.isDistLock());
}

TEST_F(CameraConfigFlagsTest, IsPitchLimitSetGet) {
  EXPECT_TRUE(flags_.isPitchLimit());
  flags_.isPitchLimit(false);
  EXPECT_FALSE(flags_.isPitchLimit());
  flags_.isPitchLimit(true);
  EXPECT_TRUE(flags_.isPitchLimit());
}

TEST_F(CameraConfigFlagsTest, IsPitchLockSetGet) {
  EXPECT_FALSE(flags_.isPitchLock());
  flags_.isPitchLock(true);
  EXPECT_TRUE(flags_.isPitchLock());
  flags_.isPitchLock(false);
  EXPECT_FALSE(flags_.isPitchLock());
}

TEST_F(CameraConfigFlagsTest, IsKeyboardEnabledSetGet) {
  EXPECT_TRUE(flags_.isKeyboardEnabled());
  flags_.isKeyboardEnabled(false);
  EXPECT_FALSE(flags_.isKeyboardEnabled());
  flags_.isKeyboardEnabled(true);
  EXPECT_TRUE(flags_.isKeyboardEnabled());
}

TEST_F(CameraConfigFlagsTest, IsMouseEnabledSetGet) {
  EXPECT_TRUE(flags_.isMouseEnabled());
  flags_.isMouseEnabled(false);
  EXPECT_FALSE(flags_.isMouseEnabled());
  flags_.isMouseEnabled(true);
  EXPECT_TRUE(flags_.isMouseEnabled());
}

TEST_F(CameraConfigFlagsTest, MultipleFlagsCanBeSetIndependently) {
  flags_.isAngleLock(true);
  flags_.isDistLock(true);
  flags_.isPitchLock(true);
  flags_.isKeyboardEnabled(false);
  flags_.isMouseEnabled(false);

  EXPECT_TRUE(flags_.isAngleLock());
  EXPECT_TRUE(flags_.isDistLock());
  EXPECT_TRUE(flags_.isPitchLock());
  EXPECT_FALSE(flags_.isKeyboardEnabled());
  EXPECT_FALSE(flags_.isMouseEnabled());
  // Other flags should remain unchanged
  EXPECT_TRUE(flags_.isDistLimit());
  EXPECT_TRUE(flags_.isPitchLimit());
}

TEST_F(CameraConfigFlagsTest, SettingFlagDoesNotAffectOthers) {
  CameraConfig::Flags clean;
  clean.isAngleLock(true);

  // Only angle lock should change
  EXPECT_TRUE(clean.isAngleLock());
  EXPECT_FALSE(clean.isDistLock());
  EXPECT_FALSE(clean.isPitchLock());
  EXPECT_TRUE(clean.isDistLimit());
  EXPECT_TRUE(clean.isPitchLimit());
  EXPECT_TRUE(clean.isKeyboardEnabled());
  EXPECT_TRUE(clean.isMouseEnabled());
}

// CameraConfig struct tests
class CameraConfigTest : public ::testing::Test {};

TEST_F(CameraConfigTest, SmoothValuesHaveDefaultZero) {
  CameraConfig config;
  EXPECT_FLOAT_EQ(0.0f, config.angle.current());
  EXPECT_FLOAT_EQ(0.0f, config.dist.current());
  EXPECT_FLOAT_EQ(0.0f, config.pitch.current());
}

// CameraKeybindings struct tests
class CameraKeybindingsTest : public ::testing::Test {};

TEST_F(CameraKeybindingsTest, DefaultKeysAreUnspecified) {
  CameraKeybindings kb;
  EXPECT_EQ(Key::Unspecified, kb.anglNegK);
  EXPECT_EQ(Key::Unspecified, kb.anglPosK);
  EXPECT_EQ(Key::Unspecified, kb.dstDecK);
  EXPECT_EQ(Key::Unspecified, kb.dstIncK);
  EXPECT_EQ(Key::Unspecified, kb.pitchNegK);
  EXPECT_EQ(Key::Unspecified, kb.pitchPosK);
  EXPECT_EQ(Key::Unspecified, kb.backwardK);
  EXPECT_EQ(Key::Unspecified, kb.forwardK);
  EXPECT_EQ(Key::Unspecified, kb.leftK);
  EXPECT_EQ(Key::Unspecified, kb.rightK);
}

TEST_F(CameraKeybindingsTest, DefaultMouseButtonsAreUnspecified) {
  CameraKeybindings kb;
  EXPECT_EQ(MouseBtn::Unspecified, kb.angleM);
  EXPECT_EQ(MouseBtn::Unspecified, kb.pitchM);
  EXPECT_EQ(MouseBtn::Unspecified, kb.trnslM);
}

TEST_F(CameraKeybindingsTest, DefaultSpeedValuesAreZero) {
  CameraKeybindings kb;
  EXPECT_FLOAT_EQ(0.0f, kb.anglSpdK);
  EXPECT_FLOAT_EQ(0.0f, kb.anglSpdM);
  EXPECT_FLOAT_EQ(0.0f, kb.dstSpdK);
  EXPECT_FLOAT_EQ(0.0f, kb.dstSpdM);
  EXPECT_FLOAT_EQ(0.0f, kb.pitchSpdK);
  EXPECT_FLOAT_EQ(0.0f, kb.pitchSpdM);
  EXPECT_FLOAT_EQ(0.0f, kb.trnslSpdK);
  EXPECT_FLOAT_EQ(0.0f, kb.trnslSpdM);
}

TEST_F(CameraKeybindingsTest, CanSetAllValues) {
  CameraKeybindings kb;
  kb.anglSpdK = 1.0f;
  kb.anglSpdM = 2.0f;
  kb.forwardK = Key::W;
  kb.backwardK = Key::S;
  kb.leftK = Key::A;
  kb.rightK = Key::D;
  kb.trnslM = MouseBtn::Middle;

  EXPECT_FLOAT_EQ(1.0f, kb.anglSpdK);
  EXPECT_FLOAT_EQ(2.0f, kb.anglSpdM);
  EXPECT_EQ(Key::W, kb.forwardK);
  EXPECT_EQ(Key::S, kb.backwardK);
  EXPECT_EQ(Key::A, kb.leftK);
  EXPECT_EQ(Key::D, kb.rightK);
  EXPECT_EQ(MouseBtn::Middle, kb.trnslM);
}

}  // namespace
}  // namespace uinta
