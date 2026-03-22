#include "uinta/input/input_system.h"

#include <gtest/gtest.h>

#include "./utils.h"
#include "uinta/engine/service_registry.h"
#include "uinta/input.h"
#include "uinta/input/input_token.h"
#include "uinta/mock/mock_platform.h"

namespace uinta {

class InputSystemTest : public UintaTestF {
 protected:
  void SetUp() override {
    serviceRegistry_.registerService<const OpenGLApi>(&gl);
    serviceRegistry_.registerService<const ArgsProcessor>(&args);
    platform_ = std::make_unique<MockPlatform>();
    engine_ = std::make_unique<Engine>(Engine::Params{
        .serviceRegistry = &serviceRegistry_,
        .platform = platform_.get(),
        .appConfig = &appConfig,
        .locale = Locale::EnUs,
    });
    system_ = std::make_unique<InputSystem>(engine_.get());
  }

  void TearDown() override {
    system_.reset();
    engine_.reset();
    platform_.reset();
  }

  Input* input() noexcept { return engine_->input(); }

  void simulateKeyPress(InputKey key, InputMod mods = 0) {
    KeyboardEvent event{.key = key, .scancode = 0, .action = ACTION_PRESS, .mods = mods};
    auto _ = input()->onKey(event);
  }

  // Helper to simulate key release
  void simulateKeyRelease(InputKey key, InputMod mods = 0) {
    KeyboardEvent event{.key = key, .scancode = 0, .action = ACTION_RELEASE, .mods = mods};
    auto _ = input()->onKey(event);
  }

  // Helper to simulate mouse button press
  void simulateMousePress(MouseButton button, InputMod mods = 0) {
    MouseEvent event{.x = 0, .y = 0, .button = button, .action = ACTION_PRESS, .mods = mods};
    auto _ = input()->onMouseButton(event);
  }

  // Helper to simulate mouse button release
  void simulateMouseRelease(MouseButton button, InputMod mods = 0) {
    MouseEvent event{.x = 0, .y = 0, .button = button, .action = ACTION_RELEASE, .mods = mods};
    auto _ = input()->onMouseButton(event);
  }

  // Helper to simulate mouse movement
  void simulateMouseMove(f32 x, f32 y) {
    MouseEvent event{.x = x, .y = y, .button = -1, .action = -1, .mods = 0};
    auto _ = input()->onCursorPos(event);
  }

  // Helper to simulate mouse scroll
  void simulateMouseScroll(f32 dx, f32 dy) {
    MouseEvent event{.x = dx, .y = dy, .button = _::InputUnknown, .action = _::InputUnknown, .mods = 0};
    auto _ = input()->onScroll(event);
  }

  ServiceRegistry serviceRegistry_;
  std::unique_ptr<MockPlatform> platform_;
  std::unique_ptr<Engine> engine_;
  std::unique_ptr<InputSystem> system_;
};

TEST_F(InputSystemTest, SubscribeKeySimplePress) {
  int callCount = 0;
  Key receivedKey = Key::Space;
  Action receivedAction = Action::Release;

  auto _ = system_->subscribeKey(Key::W | Action::Press, [&](const KeyInfo& info) {
    callCount++;
    receivedKey = info.key;
    receivedAction = info.action;
  });

  // Simulate key press
  simulateKeyPress(KEY_W);
  system_->update(0.016);

  EXPECT_EQ(callCount, 1);
  EXPECT_EQ(receivedKey, Key::W);
  EXPECT_EQ(receivedAction, Action::Press);
}

TEST_F(InputSystemTest, SubscribeKeyWithModifier) {
  int callCount = 0;
  Mod receivedMods = Mod::None;

  auto _ = system_->subscribeKey(Key::S | Action::Press | Mod::Shift, [&](const KeyInfo& info) {
    callCount++;
    receivedMods = info.mods;
  });

  // Press without modifier - should not trigger
  simulateKeyPress(KEY_S);
  system_->update(0.016);
  EXPECT_EQ(callCount, 0);

  input()->reset();

  // Press with shift - should trigger
  simulateKeyPress(KEY_S, MOD_SHIFT);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);
  EXPECT_EQ(receivedMods, Mod::Shift);
}

TEST_F(InputSystemTest, ExactModifierMatching) {
  int callCount = 0;

  // Subscribe to Shift only
  auto _ = system_->subscribeKey(Key::A | Action::Press | Mod::Shift, [&](const auto&) { callCount++; });

  // Press with Shift + Control - should NOT trigger (extra modifier)
  simulateKeyPress(KEY_A, MOD_SHIFT | MOD_CONTROL);
  system_->update(0.016);
  EXPECT_EQ(callCount, 0);

  input()->reset();

  // Press with just Shift - should trigger
  simulateKeyPress(KEY_A, MOD_SHIFT);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);
}

TEST_F(InputSystemTest, MultipleModifiers) {
  int callCount = 0;

  auto _ = system_->subscribeKey(Key::Q | Action::Press | Mod::Control | Mod::Shift, [&](const auto&) { callCount++; });

  // Press with both modifiers
  simulateKeyPress(KEY_Q, MOD_CONTROL | MOD_SHIFT);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);

  input()->reset();

  // Press with only one modifier - should not trigger
  simulateKeyPress(KEY_Q, MOD_CONTROL);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);  // Still 1, no new trigger
}

TEST_F(InputSystemTest, KeyRelease) {
  int callCount = 0;
  Action receivedAction = Action::Press;

  auto _ = system_->subscribeKey(Key::Escape | Action::Release, [&](const KeyInfo& info) {
    callCount++;
    receivedAction = info.action;
  });

  // Press should not trigger
  simulateKeyPress(KEY_ESCAPE);
  system_->update(0.016);
  EXPECT_EQ(callCount, 0);

  // Release should trigger
  simulateKeyRelease(KEY_ESCAPE);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);
  EXPECT_EQ(receivedAction, Action::Release);
}

TEST_F(InputSystemTest, MultipleSubscriptionsToSameKey) {
  int count1 = 0;
  int count2 = 0;

  auto _ = system_->subscribeKey(Key::W | Action::Press, [&](const auto&) { count1++; });
  _ = system_->subscribeKey(Key::W | Action::Press, [&](const auto&) { count2++; });

  simulateKeyPress(KEY_W);
  system_->update(0.016);

  EXPECT_EQ(count1, 1);
  EXPECT_EQ(count2, 1);
}

TEST_F(InputSystemTest, UnsubscribeKey) {
  int callCount = 0;

  auto handle = system_->subscribeKey(Key::W | Action::Press, [&](const auto&) { callCount++; });

  // Should trigger before unsubscribe
  simulateKeyPress(KEY_W);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);

  input()->reset();

  // Unsubscribe
  system_->unsubscribe(handle);

  // Should not trigger after unsubscribe
  simulateKeyPress(KEY_W);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);  // Still 1, no new trigger
}

TEST_F(InputSystemTest, SubscribeMouseButton) {
  int callCount = 0;
  MouseBtn receivedButton = MouseBtn::Right;

  auto _ = system_->subscribeMouse(MouseBtn::Left | Action::Press, [&](const MouseButtonInfo& info) {
    callCount++;
    receivedButton = info.button;
  });

  simulateMousePress(MOUSE_BUTTON_LEFT);
  system_->update(0.016);

  EXPECT_EQ(callCount, 1);
  EXPECT_EQ(receivedButton, MouseBtn::Left);
}

TEST_F(InputSystemTest, MouseButtonWithModifier) {
  int callCount = 0;

  auto _ = system_->subscribeMouse(MouseBtn::Right | Action::Press | Mod::Alt, [&](const auto&) { callCount++; });

  // Press without modifier - should not trigger
  simulateMousePress(MOUSE_BUTTON_RIGHT);
  system_->update(0.016);
  EXPECT_EQ(callCount, 0);

  input()->reset();

  // Press with Alt - should trigger
  simulateMousePress(MOUSE_BUTTON_RIGHT, MOD_ALT);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);
}

TEST_F(InputSystemTest, MouseMove) {
  int callCount = 0;
  f32 receivedX = 0;
  f32 receivedY = 0;
  f32 receivedDx = 0;
  f32 receivedDy = 0;

  auto _ = system_->subscribeMouseMove([&](const MouseMoveInfo& info) {
    callCount++;
    receivedX = info.x;
    receivedY = info.y;
    receivedDx = info.dx;
    receivedDy = info.dy;
  });

  // Initial position
  simulateMouseMove(100.0f, 200.0f);
  system_->update(0.016);

  EXPECT_EQ(callCount, 1);
  EXPECT_FLOAT_EQ(receivedX, 100.0f);
  EXPECT_FLOAT_EQ(receivedY, 200.0f);

  // Move to new position
  simulateMouseMove(150.0f, 250.0f);
  system_->update(0.016);

  EXPECT_EQ(callCount, 2);
  EXPECT_FLOAT_EQ(receivedX, 150.0f);
  EXPECT_FLOAT_EQ(receivedY, 250.0f);
  EXPECT_FLOAT_EQ(receivedDx, 50.0f);
  EXPECT_FLOAT_EQ(receivedDy, 50.0f);
}

TEST_F(InputSystemTest, MouseScroll) {
  int callCount = 0;
  f32 receivedDx = 0;
  f32 receivedDy = 0;

  auto _ = system_->subscribeMouseScroll([&](const MouseScrollInfo& info) {
    callCount++;
    receivedDx = info.dx;
    receivedDy = info.dy;
  });

  simulateMouseScroll(1.0f, -2.0f);
  system_->update(0.016);

  EXPECT_EQ(callCount, 1);
  EXPECT_FLOAT_EQ(receivedDx, 1.0f);
  EXPECT_FLOAT_EQ(receivedDy, -2.0f);
}

TEST_F(InputSystemTest, UnsubscribeMouseMove) {
  int callCount = 0;

  auto handle = system_->subscribeMouseMove([&](const auto&) { callCount++; });

  simulateMouseMove(100.0f, 200.0f);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);

  // Unsubscribe
  system_->unsubscribe(handle);

  simulateMouseMove(150.0f, 250.0f);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);  // Still 1, no new trigger
}

TEST_F(InputSystemTest, MultipleMouseMoveSubscriptions) {
  int count1 = 0;
  int count2 = 0;

  auto _ = system_->subscribeMouseMove([&](const auto&) { count1++; });
  _ = system_->subscribeMouseMove([&](const auto&) { count2++; });

  simulateMouseMove(100.0f, 200.0f);
  system_->update(0.016);

  EXPECT_EQ(count1, 1);
  EXPECT_EQ(count2, 1);
}

TEST_F(InputSystemTest, NoModifierMatchesModNone) {
  int callCount = 0;

  // Subscribe without modifiers (implicitly Mod::None)
  auto _ = system_->subscribeKey(Key::W | Action::Press, [&](const auto&) { callCount++; });

  // Press without modifiers - should trigger
  simulateKeyPress(KEY_W);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);

  input()->reset();

  // Press with modifier - should NOT trigger
  simulateKeyPress(KEY_W, MOD_SHIFT);
  system_->update(0.016);
  EXPECT_EQ(callCount, 1);  // Still 1, no new trigger
}

TEST_F(InputSystemTest, UniqueHandleIds) {
  auto h1 = system_->subscribeKey(Key::W | Action::Press, [](const auto&) {});
  auto h2 = system_->subscribeKey(Key::S | Action::Press, [](const auto&) {});
  auto h3 = system_->subscribeMouseMove([](const auto&) {});

  EXPECT_NE(h1.id, h2.id);
  EXPECT_NE(h1.id, h3.id);
  EXPECT_NE(h2.id, h3.id);
}

// Token validation tests
TEST(TokenValidationTest, ValidKeyToken) {
  // Key + Action is valid
  u32 token = Key::W | Action::Press;
  EXPECT_TRUE(isValidKeyToken(token));
}

TEST(TokenValidationTest, ValidKeyTokenWithModifier) {
  // Key + action + modifier is valid
  u32 token = Key::S | Action::Release | Mod::Control;
  EXPECT_TRUE(isValidKeyToken(token));
}

TEST(TokenValidationTest, ValidKeyTokenMultipleModifiers) {
  // Key + action + multiple modifiers is valid
  u32 token = Key::A | Action::Press | Mod::Control | Mod::Shift;
  EXPECT_TRUE(isValidKeyToken(token));
}

TEST(TokenValidationTest, InvalidKeyTokenMissingKey) {
  // Action only (no key) is invalid
  u32 token = static_cast<u32>(Action::Press);
  EXPECT_FALSE(isValidKeyToken(token));
}

TEST(TokenValidationTest, InvalidKeyTokenMissingAction) {
  // Key only (no action) is invalid
  u32 token = static_cast<u32>(Key::W);
  EXPECT_FALSE(isValidKeyToken(token));
}

TEST(TokenValidationTest, InvalidKeyTokenEmpty) {
  // Empty token is invalid
  u32 token = 0;
  EXPECT_FALSE(isValidKeyToken(token));
}

TEST(TokenValidationTest, ValidMouseButtonToken) {
  // Button + Action is valid
  u32 token = MouseBtn::Left | Action::Press;
  EXPECT_TRUE(isValidMouseButtonToken(token));
}

TEST(TokenValidationTest, ValidMouseButtonTokenWithModifier) {
  // Button + action + modifier is valid
  u32 token = MouseBtn::Middle | Action::Press | Mod::Shift;
  EXPECT_TRUE(isValidMouseButtonToken(token));
}

TEST(TokenValidationTest, ValidMouseButtonTokenMultipleModifiers) {
  // Button + action + multiple modifiers is valid
  u32 token = MouseBtn::Right | Action::Release | Mod::Control | Mod::Alt;
  EXPECT_TRUE(isValidMouseButtonToken(token));
}

TEST(TokenValidationTest, ValidMouseButtonTokenWithLeftButton) {
  // Left button (0) with action is still valid
  u32 token = static_cast<u32>(MouseBtn::Left) | static_cast<u32>(Action::Press);
  EXPECT_TRUE(isValidMouseButtonToken(token));
}

TEST(TokenValidationTest, InvalidMouseButtonTokenMissingAction) {
  // Button only (no action) is invalid
  u32 token = static_cast<u32>(MouseBtn::Left);
  EXPECT_FALSE(isValidMouseButtonToken(token));
}

TEST(TokenValidationTest, InvalidMouseButtonTokenEmpty) {
  // Empty token is invalid
  u32 token = 0;
  EXPECT_FALSE(isValidMouseButtonToken(token));
}

TEST(TokenValidationTest, ComponentExtraction) {
  u32 token = Key::W | Action::Press | Mod::Control;

  // Verify extraction functions work correctly
  EXPECT_EQ(extractKey(token), Key::W);
  EXPECT_EQ(extractAction(token), Action::Press);
  EXPECT_EQ(extractMods(token), Mod::Control);

  // Verify validation with extracted components
  EXPECT_NE(extractKey(token), Key::Space);
  EXPECT_NE(extractAction(token), Action::Release);
  EXPECT_NE(extractMods(token), Mod::Shift);
}

}  // namespace uinta
