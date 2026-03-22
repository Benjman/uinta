#include "uinta/input/input_system.h"

#include <gtest/gtest.h>

#include "./utils.h"
#include "uinta/input.h"
#include "uinta/input/input_token.h"
#include "uinta/mock/mock_platform.h"

namespace uinta {

class InputSystemTest : public UintaTestF {
 protected:
  void SetUp() override {
    platform_ = std::make_unique<MockPlatform>();
    engine_ = std::make_unique<Engine>(Engine::Params{
        .platform = platform_.get(),
        .appConfig = &appConfig,
        .args = &args,
        .gl = &gl,
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
    KeyboardEvent event{
        .key = key, .scancode = 0, .action = ACTION_PRESS, .mods = mods};
    auto _ = input()->onKey(event);
  }

  // Helper to simulate key release
  void simulateKeyRelease(InputKey key, InputMod mods = 0) {
    KeyboardEvent event{
        .key = key, .scancode = 0, .action = ACTION_RELEASE, .mods = mods};
    auto _ = input()->onKey(event);
  }

  // Helper to simulate mouse button press
  void simulateMousePress(MouseButton button, InputMod mods = 0) {
    MouseEvent event{
        .x = 0, .y = 0, .button = button, .action = ACTION_PRESS, .mods = mods};
    auto _ = input()->onMouseButton(event);
  }

  // Helper to simulate mouse button release
  void simulateMouseRelease(MouseButton button, InputMod mods = 0) {
    MouseEvent event{.x = 0,
                     .y = 0,
                     .button = button,
                     .action = ACTION_RELEASE,
                     .mods = mods};
    auto _ = input()->onMouseButton(event);
  }

  // Helper to simulate mouse movement
  void simulateMouseMove(f32 x, f32 y) {
    MouseEvent event{.x = x, .y = y, .button = -1, .action = -1, .mods = 0};
    auto _ = input()->onCursorPos(event);
  }

  // Helper to simulate mouse scroll
  void simulateMouseScroll(f32 dx, f32 dy) {
    MouseEvent event{.x = dx,
                     .y = dy,
                     .button = _::InputUnknown,
                     .action = _::InputUnknown,
                     .mods = 0};
    auto _ = input()->onScroll(event);
  }

  std::unique_ptr<MockPlatform> platform_;
  std::unique_ptr<Engine> engine_;
  std::unique_ptr<InputSystem> system_;
};

TEST_F(InputSystemTest, SubscribeKeySimplePress) {
  int callCount = 0;
  Key receivedKey = Key::Space;
  Action receivedAction = Action::Release;

  auto _ = system_->subscribeKey(Key::W | Action::Press,
                                 [&](Key key, Action action, Mod /*mods*/) {
                                   callCount++;
                                   receivedKey = key;
                                   receivedAction = action;
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

  auto _ = system_->subscribeKey(Key::S | Action::Press | Mod::Shift,
                                 [&](Key /*key*/, Action /*action*/, Mod mods) {
                                   callCount++;
                                   receivedMods = mods;
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
  auto _ = system_->subscribeKey(Key::A | Action::Press | Mod::Shift,
                                 [&](Key, Action, Mod) { callCount++; });

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

  auto _ =
      system_->subscribeKey(Key::Q | Action::Press | Mod::Control | Mod::Shift,
                            [&](Key, Action, Mod) { callCount++; });

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

  auto _ = system_->subscribeKey(Key::Escape | Action::Release,
                                 [&](Key, Action action, Mod) {
                                   callCount++;
                                   receivedAction = action;
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

  auto _ = system_->subscribeKey(Key::W | Action::Press,
                                 [&](Key, Action, Mod) { count1++; });
  _ = system_->subscribeKey(Key::W | Action::Press,
                            [&](Key, Action, Mod) { count2++; });

  simulateKeyPress(KEY_W);
  system_->update(0.016);

  EXPECT_EQ(count1, 1);
  EXPECT_EQ(count2, 1);
}

TEST_F(InputSystemTest, UnsubscribeKey) {
  int callCount = 0;

  auto handle = system_->subscribeKey(Key::W | Action::Press,
                                      [&](Key, Action, Mod) { callCount++; });

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

  auto _ = system_->subscribeMouse(MouseBtn::Left | Action::Press,
                                   [&](MouseBtn button, Action, Mod) {
                                     callCount++;
                                     receivedButton = button;
                                   });

  simulateMousePress(MOUSE_BUTTON_LEFT);
  system_->update(0.016);

  EXPECT_EQ(callCount, 1);
  EXPECT_EQ(receivedButton, MouseBtn::Left);
}

TEST_F(InputSystemTest, MouseButtonWithModifier) {
  int callCount = 0;

  auto _ = system_->subscribeMouse(MouseBtn::Right | Action::Press | Mod::Alt,
                                   [&](MouseBtn, Action, Mod) { callCount++; });

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

  auto _ = system_->subscribeMouseMove([&](f32 x, f32 y, f32 dx, f32 dy) {
    callCount++;
    receivedX = x;
    receivedY = y;
    receivedDx = dx;
    receivedDy = dy;
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

  auto _ = system_->subscribeMouseScroll([&](f32 dx, f32 dy) {
    callCount++;
    receivedDx = dx;
    receivedDy = dy;
  });

  simulateMouseScroll(1.0f, -2.0f);
  system_->update(0.016);

  EXPECT_EQ(callCount, 1);
  EXPECT_FLOAT_EQ(receivedDx, 1.0f);
  EXPECT_FLOAT_EQ(receivedDy, -2.0f);
}

TEST_F(InputSystemTest, UnsubscribeMouseMove) {
  int callCount = 0;

  auto handle =
      system_->subscribeMouseMove([&](f32, f32, f32, f32) { callCount++; });

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

  auto _ = system_->subscribeMouseMove([&](f32, f32, f32, f32) { count1++; });
  _ = system_->subscribeMouseMove([&](f32, f32, f32, f32) { count2++; });

  simulateMouseMove(100.0f, 200.0f);
  system_->update(0.016);

  EXPECT_EQ(count1, 1);
  EXPECT_EQ(count2, 1);
}

TEST_F(InputSystemTest, NoModifierMatchesModNone) {
  int callCount = 0;

  // Subscribe without modifiers (implicitly Mod::None)
  auto _ = system_->subscribeKey(Key::W | Action::Press,
                                 [&](Key, Action, Mod) { callCount++; });

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
  auto h1 =
      system_->subscribeKey(Key::W | Action::Press, [](Key, Action, Mod) {});
  auto h2 =
      system_->subscribeKey(Key::S | Action::Press, [](Key, Action, Mod) {});
  auto h3 = system_->subscribeMouseMove([](f32, f32, f32, f32) {});

  EXPECT_NE(h1.id, h2.id);
  EXPECT_NE(h1.id, h3.id);
  EXPECT_NE(h2.id, h3.id);
}

}  // namespace uinta
