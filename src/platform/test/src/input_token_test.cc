#include "uinta/input/input_token.h"

#include <gtest/gtest.h>

#include "uinta/input/fwd.h"

namespace uinta {

TEST(InputTokenTest, KeyActionCombination) {
  // Test: Key::W | Action::Press
  u32 token = Key::W | Action::Press;

  // Verify key extraction
  EXPECT_EQ(extractKey(token), Key::W);

  // Verify action extraction
  EXPECT_EQ(extractAction(token), Action::Press);

  // Verify no modifiers
  EXPECT_EQ(extractMods(token), Mod::None);

  // Verify the token structure
  EXPECT_EQ(token & 0x0000FFFF, 87u);          // Key::W = 87
  EXPECT_EQ(token & 0x000F0000, 0x00010000u);  // Action::Press bit 16
}

TEST(InputTokenTest, KeyActionModifierCombination) {
  // Test: Key::S | Action::Press | Mod::Shift
  u32 token = Key::S | Action::Press | Mod::Shift;

  EXPECT_EQ(extractKey(token), Key::S);
  EXPECT_EQ(extractAction(token), Action::Press);
  EXPECT_EQ(extractMods(token), Mod::Shift);
}

TEST(InputTokenTest, MultipleModifiers) {
  // Test: Key::Q | Action::Release | Mod::Control | Mod::Shift
  u32 token = Key::Q | Action::Release | Mod::Control | Mod::Shift;

  EXPECT_EQ(extractKey(token), Key::Q);
  EXPECT_EQ(extractAction(token), Action::Release);

  // Verify both modifiers are set
  u32 modBits = static_cast<u32>(extractMods(token));
  EXPECT_NE(modBits & static_cast<u32>(Mod::Control), 0u);
  EXPECT_NE(modBits & static_cast<u32>(Mod::Shift), 0u);
}

TEST(InputTokenTest, MouseButtonCombination) {
  // Test: MouseBtn::Left | Action::Press
  u32 token = MouseBtn::Left | Action::Press;

  EXPECT_EQ(extractMouseButton(token), MouseBtn::Left);
  EXPECT_EQ(extractAction(token), Action::Press);
  EXPECT_EQ(extractMods(token), Mod::None);
}

TEST(InputTokenTest, MouseButtonWithModifiers) {
  // Test: MouseBtn::Right | Action::Press | Mod::Alt
  u32 token = MouseBtn::Right | Action::Press | Mod::Alt;

  EXPECT_EQ(extractMouseButton(token), MouseBtn::Right);
  EXPECT_EQ(extractAction(token), Action::Press);
  EXPECT_EQ(extractMods(token), Mod::Alt);
}

TEST(InputTokenTest, BitfieldNonOverlapping) {
  // Ensure different bit ranges don't overlap
  u32 key = static_cast<u32>(Key::W);            // bits 0-15
  u32 action = static_cast<u32>(Action::Press);  // bits 16-19
  u32 mod = static_cast<u32>(Mod::Shift);        // bits 20-27

  EXPECT_EQ(key & 0xFFFF0000u, 0u);     // Key uses only lower 16 bits
  EXPECT_EQ(action & 0xFFF0FFFFu, 0u);  // Action uses only bits 16-19
  EXPECT_EQ(mod & 0xF00FFFFFu, 0u);     // Mod uses only bits 20-27
}

TEST(InputTokenTest, AllModifiersCombined) {
  // Test all modifiers together
  u32 token = Key::A | Action::Press | Mod::Shift | Mod::Control | Mod::Alt |
              Mod::Super | Mod::CapsLock | Mod::NumLock;

  u32 modBits = static_cast<u32>(extractMods(token));
  EXPECT_NE(modBits & static_cast<u32>(Mod::Shift), 0u);
  EXPECT_NE(modBits & static_cast<u32>(Mod::Control), 0u);
  EXPECT_NE(modBits & static_cast<u32>(Mod::Alt), 0u);
  EXPECT_NE(modBits & static_cast<u32>(Mod::Super), 0u);
  EXPECT_NE(modBits & static_cast<u32>(Mod::CapsLock), 0u);
  EXPECT_NE(modBits & static_cast<u32>(Mod::NumLock), 0u);
}

TEST(InputTokenTest, MultipleActions) {
  // Test combining multiple actions (for subscription to multiple actions)
  u32 multiAction =
      static_cast<u32>(Action::Press) | static_cast<u32>(Action::Release);
  u32 token = static_cast<u32>(Key::W) | multiAction;

  // Check that both action bits are set
  u32 extractedAction = static_cast<u32>(extractAction(token));
  EXPECT_NE(extractedAction & static_cast<u32>(Action::Press), 0u);
  EXPECT_NE(extractedAction & static_cast<u32>(Action::Release), 0u);
}

TEST(InputTokenTest, SubscriptionHandleEquality) {
  SubscriptionHandle h1{42};
  SubscriptionHandle h2{42};
  SubscriptionHandle h3{99};

  EXPECT_EQ(h1, h2);
  EXPECT_NE(h1, h3);
}

TEST(InputTokenTest, ConversionToKey) {
  InputKey inputKey = KEY_W;
  Key key = toKey(inputKey);
  InputKey convertedBack = fromKey(key);

  EXPECT_EQ(static_cast<u32>(key), 87u);  // W = 87
  EXPECT_EQ(convertedBack, inputKey);
}

TEST(InputTokenTest, ConversionToAction) {
  // Test each action conversion
  Action press = toAction(ACTION_PRESS);
  Action release = toAction(ACTION_RELEASE);
  Action repeat = toAction(ACTION_REPEAT);

  EXPECT_EQ(press, Action::Press);
  EXPECT_EQ(release, Action::Release);
  EXPECT_EQ(repeat, Action::Repeat);

  // Test conversion back
  EXPECT_EQ(fromAction(Action::Press), ACTION_PRESS);
  EXPECT_EQ(fromAction(Action::Release), ACTION_RELEASE);
  EXPECT_EQ(fromAction(Action::Repeat), ACTION_REPEAT);
}

TEST(InputTokenTest, ConversionToMod) {
  InputMod inputMod = MOD_SHIFT | MOD_CONTROL;
  Mod mod = toMod(inputMod);
  InputMod convertedBack = fromMod(mod);

  // Verify the mod has the correct bits set
  u32 modBits = static_cast<u32>(mod);
  EXPECT_NE(modBits & static_cast<u32>(Mod::Shift), 0u);
  EXPECT_NE(modBits & static_cast<u32>(Mod::Control), 0u);

  // Verify conversion back preserves the flags
  EXPECT_NE(convertedBack & MOD_SHIFT, 0);
  EXPECT_NE(convertedBack & MOD_CONTROL, 0);
}

TEST(InputTokenTest, ConversionAllModifiers) {
  // Test all modifier conversions
  InputMod allInputMods = MOD_SHIFT | MOD_CONTROL | MOD_ALT | MOD_SUPER |
                          MOD_CAPS_LOCK | MOD_NUM_LOCK;
  Mod mod = toMod(allInputMods);
  InputMod convertedBack = fromMod(mod);

  EXPECT_NE(convertedBack & MOD_SHIFT, 0);
  EXPECT_NE(convertedBack & MOD_CONTROL, 0);
  EXPECT_NE(convertedBack & MOD_ALT, 0);
  EXPECT_NE(convertedBack & MOD_SUPER, 0);
  EXPECT_NE(convertedBack & MOD_CAPS_LOCK, 0);
  EXPECT_NE(convertedBack & MOD_NUM_LOCK, 0);
}

TEST(InputTokenTest, MouseButtonConversion) {
  MouseButton inputButton = MOUSE_BUTTON_LEFT;
  MouseBtn btn = toMouseButton(inputButton);
  MouseButton convertedBack = fromMouseButton(btn);

  EXPECT_EQ(convertedBack, inputButton);
}

}  // namespace uinta