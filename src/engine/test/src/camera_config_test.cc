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

TEST_F(CameraKeybindingsTest, DefaultInputKeysAreUnknown) {
  CameraKeybindings kb;
  EXPECT_EQ(_::InputUnknown, kb.anglNegK);
  EXPECT_EQ(_::InputUnknown, kb.anglPosK);
  EXPECT_EQ(_::InputUnknown, kb.dstDecK);
  EXPECT_EQ(_::InputUnknown, kb.dstIncK);
  EXPECT_EQ(_::InputUnknown, kb.pitchNegK);
  EXPECT_EQ(_::InputUnknown, kb.pitchPosK);
  EXPECT_EQ(_::InputUnknown, kb.backwardK);
  EXPECT_EQ(_::InputUnknown, kb.forwardK);
  EXPECT_EQ(_::InputUnknown, kb.leftK);
  EXPECT_EQ(_::InputUnknown, kb.rightK);
}

TEST_F(CameraKeybindingsTest, DefaultMouseButtonsAreUnknown) {
  CameraKeybindings kb;
  EXPECT_EQ(_::InputUnknown, kb.angleM);
  EXPECT_EQ(_::InputUnknown, kb.pitchM);
  EXPECT_EQ(_::InputUnknown, kb.trnslM);
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
  kb.forwardK = KEY_W;
  kb.backwardK = KEY_S;
  kb.leftK = KEY_A;
  kb.rightK = KEY_D;
  kb.trnslM = MOUSE_BUTTON_MIDDLE;

  EXPECT_FLOAT_EQ(1.0f, kb.anglSpdK);
  EXPECT_FLOAT_EQ(2.0f, kb.anglSpdM);
  EXPECT_EQ(KEY_W, kb.forwardK);
  EXPECT_EQ(KEY_S, kb.backwardK);
  EXPECT_EQ(KEY_A, kb.leftK);
  EXPECT_EQ(KEY_D, kb.rightK);
  EXPECT_EQ(MOUSE_BUTTON_MIDDLE, kb.trnslM);
}

}  // namespace
}  // namespace uinta
