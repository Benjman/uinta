#include "uinta/input.h"

#include <gtest/gtest.h>

namespace uinta {

class InputTest : public testing::Test {
 protected:
  Input input;
};

TEST_F(InputTest, InitialState) {
  ASSERT_EQ(_::InputStorageSize, input.keysDown().size());
  ASSERT_EQ(_::InputStorageSize, input.keysPressed().size());
  ASSERT_EQ(_::InputStorageSize, input.keysReleased().size());
  ASSERT_EQ(_::InputStorageSize, input.keysRepeated().size());
  ASSERT_EQ(_::InputStorageSize, input.mouseDown().size());
  ASSERT_EQ(_::InputStorageSize, input.mousePressed().size());
  ASSERT_EQ(_::InputStorageSize, input.mouseReleased().size());
  ASSERT_EQ(0, input.mods());
  ASSERT_EQ(0, input.cursordx());
  ASSERT_EQ(0, input.cursordy());
  ASSERT_EQ(0, input.cursorx());
  ASSERT_EQ(0, input.cursory());
  ASSERT_EQ(0, input.scrolldx());
  ASSERT_EQ(0, input.scrolldy());

  for (size_t i = 0; i < _::InputStorageSize; i++)
    ASSERT_EQ(_::InputUnknown, input.keysDown().at(i));
  for (size_t i = 0; i < _::InputStorageSize; i++)
    ASSERT_EQ(_::InputUnknown, input.keysPressed().at(i));
  for (size_t i = 0; i < _::InputStorageSize; i++)
    ASSERT_EQ(_::InputUnknown, input.keysReleased().at(i));
  for (size_t i = 0; i < _::InputStorageSize; i++)
    ASSERT_EQ(_::InputUnknown, input.keysRepeated().at(i));
  for (size_t i = 0; i < _::InputStorageSize; i++)
    ASSERT_EQ(_::InputUnknown, input.mouseDown().at(i));
  for (size_t i = 0; i < _::InputStorageSize; i++)
    ASSERT_EQ(_::InputUnknown, input.mousePressed().at(i));
  for (size_t i = 0; i < _::InputStorageSize; i++)
    ASSERT_EQ(_::InputUnknown, input.mouseReleased().at(i));
}

TEST_F(InputTest, onCursorPos) {
  f32 x = 5;
  f32 y = 6;

  MouseEvent event(x, y, -1, -1, 0);
  ASSERT_TRUE(input.onCursorPos(event).ok());
  ASSERT_EQ(x, input.cursorx());
  ASSERT_EQ(y, input.cursory());
  ASSERT_EQ(x, input.cursordx());
  ASSERT_EQ(y, input.cursordy());
  ASSERT_TRUE(input.flags().isMouseMove());

  event.x = 3 * x;
  event.y = 3 * y;
  ASSERT_TRUE(input.onCursorPos(event).ok());
  ASSERT_EQ(3 * x, input.cursorx());
  ASSERT_EQ(3 * y, input.cursory());
  ASSERT_EQ(2 * x, input.cursordx());
  ASSERT_EQ(2 * y, input.cursordy());
}

TEST_F(InputTest, onKey_mods) {
  KeyboardEvent event;
  event.key = KEY_A;
  event.action = ACTION_PRESS;

  event.mods = MOD_ALT;
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_TRUE(input.altDown());
  ASSERT_EQ(MOD_ALT, input.mods() & MOD_ALT);
  ASSERT_EQ(0, input.mods() & MOD_CONTROL);
  ASSERT_EQ(0, input.mods() & MOD_SHIFT);
  ASSERT_EQ(0, input.mods() & MOD_SUPER);

  input.reset();
  event.action = ACTION_RELEASE;
  (void)input.onKey(event);
  event.action = ACTION_PRESS;

  event.mods = MOD_CONTROL;
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_TRUE(input.ctrlDown());
  ASSERT_EQ(0, input.mods() & MOD_ALT);
  ASSERT_EQ(MOD_CONTROL, input.mods() & MOD_CONTROL);
  ASSERT_EQ(0, input.mods() & MOD_SHIFT);
  ASSERT_EQ(0, input.mods() & MOD_SUPER);

  input.reset();
  event.action = ACTION_RELEASE;
  (void)input.onKey(event);
  event.action = ACTION_PRESS;

  event.mods = MOD_SHIFT;
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_TRUE(input.shiftDown());
  ASSERT_EQ(0, input.mods() & MOD_ALT);
  ASSERT_EQ(0, input.mods() & MOD_CONTROL);
  ASSERT_EQ(MOD_SHIFT, input.mods() & MOD_SHIFT);
  ASSERT_EQ(0, input.mods() & MOD_SUPER);

  input.reset();
  event.action = ACTION_RELEASE;
  (void)input.onKey(event);
  event.action = ACTION_PRESS;

  event.mods = MOD_SUPER;
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_TRUE(input.superDown());
  ASSERT_EQ(0, input.mods() & MOD_ALT);
  ASSERT_EQ(0, input.mods() & MOD_CONTROL);
  ASSERT_EQ(0, input.mods() & MOD_SHIFT);
  ASSERT_EQ(MOD_SUPER, input.mods() & MOD_SUPER);
}

TEST_F(InputTest, onKey_press) {
  KeyboardEvent event;

  event.key = KEY_A;
  event.action = ACTION_PRESS;
  for (size_t i = 0; i < _::InputStorageSize; i++) {
    ASSERT_TRUE(input.onKey(event).ok());
    ASSERT_EQ(event.key, input.keysPressed().at(i));
    ASSERT_EQ(event.key, input.keysDown().at(i));
    if (i + 1 < _::InputStorageSize) {
      ASSERT_EQ(_::InputUnknown, input.keysPressed().at(i + 1));
      ASSERT_EQ(_::InputUnknown, input.keysDown().at(i + 1));
    }
    ASSERT_TRUE(input.anyKeyPressed());
    ASSERT_TRUE(input.anyKeyDown());
    ASSERT_TRUE(input.flags().isKeyPressed());
    ASSERT_TRUE(input.flags().isKeyDown());
    event.key++;
  }
}

TEST_F(InputTest, onKey_storageFull) {
  Input input;
  KeyboardEvent event;
  event.key = KEY_A;

  event.action = ACTION_PRESS;
  for (size_t i = 0; i < _::InputStorageSize; i++) {
    ASSERT_TRUE(input.onKey(event).ok());
    event.key++;
  }
  event.key = KEY_Z;
  auto status = input.onKey(event);
  ASSERT_FALSE(status.ok());
  ASSERT_TRUE(IsResourceExhausted(status));
}

TEST_F(InputTest, onKey_release) {
  KeyboardEvent eventA;
  eventA.key = KEY_A;
  eventA.action = ACTION_PRESS;

  auto eventB = eventA;
  eventB.key = KEY_B;

  auto eventC = eventA;
  eventC.key = KEY_C;

  ASSERT_TRUE(input.onKey(eventA).ok());
  ASSERT_TRUE(input.onKey(eventB).ok());
  ASSERT_TRUE(input.onKey(eventC).ok());
  ASSERT_EQ(eventA.key, input.keysDown().at(0));
  ASSERT_EQ(eventB.key, input.keysDown().at(1));
  ASSERT_EQ(eventC.key, input.keysDown().at(2));

  eventB.action = ACTION_RELEASE;
  ASSERT_TRUE(input.onKey(eventB).ok());
  ASSERT_TRUE(input.isKeyReleased(eventB.key));
  ASSERT_EQ(eventA.key, input.keysDown().at(0));
  ASSERT_EQ(eventC.key, input.keysDown().at(1));
  ASSERT_EQ(_::InputUnknown, input.keysDown().at(2));

  eventA.action = ACTION_RELEASE;
  ASSERT_TRUE(input.onKey(eventA).ok());
  ASSERT_EQ(eventC.key, input.keysDown().at(0));
  ASSERT_EQ(_::InputUnknown, input.keysDown().at(1));
  ASSERT_EQ(_::InputUnknown, input.keysDown().at(2));

  ASSERT_TRUE(input.flags().isKeyReleased());
}

TEST_F(InputTest, onKey_repeated) {
  KeyboardEvent event;
  event.key = KEY_A;
  event.action = ACTION_REPEAT;
  for (size_t i = 0; i < _::InputStorageSize; i++) {
    ASSERT_TRUE(input.onKey(event).ok());
    ASSERT_EQ(event.key, input.keysRepeated().at(i));
    event.key++;
  }
  auto status = input.onKey(event);
  ASSERT_FALSE(status.ok());
  ASSERT_TRUE(IsResourceExhausted(status));
  ASSERT_TRUE(input.flags().isKeyRepeated());
}

TEST_F(InputTest, onKey_press_duplicate) {
  KeyboardEvent event;
  event.key = KEY_A;
  event.action = ACTION_PRESS;
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_EQ(event.key, input.keysPressed().at(0));
  ASSERT_EQ(event.key, input.keysDown().at(0));
  ASSERT_EQ(_::InputUnknown, input.keysPressed().at(1));
  ASSERT_EQ(_::InputUnknown, input.keysDown().at(1));
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_EQ(event.key, input.keysPressed().at(0));
  ASSERT_EQ(event.key, input.keysDown().at(0));
  ASSERT_EQ(_::InputUnknown, input.keysPressed().at(1));
  ASSERT_EQ(_::InputUnknown, input.keysDown().at(1));
}

TEST_F(InputTest, onKey_invalidAction) {
  KeyboardEvent event;
  event.action = _::InputUnknown;
  ASSERT_TRUE(IsNotFound(input.onKey(event)));
}

TEST_F(InputTest, onMouseButton_pressed) {
  MouseEvent event;
  event.button = MOUSE_BUTTON_LEFT;
  event.action = ACTION_PRESS;
  ASSERT_TRUE(input.onMouseButton(event).ok());
  ASSERT_TRUE(input.anyMouseDown());
  ASSERT_TRUE(input.anyMousePressed());
  ASSERT_EQ(MOUSE_BUTTON_LEFT, input.mousePressed().at(0));
  ASSERT_EQ(MOUSE_BUTTON_LEFT, input.mouseDown().at(0));
  ASSERT_TRUE(input.flags().isMousePressed());
  ASSERT_TRUE(input.flags().isMouseDown());
}

TEST_F(InputTest, onMouseButton_released) {
  MouseEvent event;
  event.button = MOUSE_BUTTON_LEFT;
  event.action = ACTION_PRESS;
  ASSERT_TRUE(input.onMouseButton(event).ok());
  ASSERT_TRUE(input.flags().isMouseDown());
  input.reset();
  event.action = ACTION_RELEASE;
  ASSERT_TRUE(input.onMouseButton(event).ok());
  ASSERT_FALSE(input.anyMouseDown());
  ASSERT_FALSE(input.anyMousePressed());
  ASSERT_EQ(MOUSE_BUTTON_LEFT, input.mouseReleased().at(0));
  ASSERT_EQ(_::InputUnknown, input.mouseDown().at(0));
  ASSERT_TRUE(input.flags().isMouseReleased());
  ASSERT_FALSE(input.flags().isMouseDown());
}

TEST_F(InputTest, onMouseButton_invalidAction) {
  MouseEvent event;
  event.action = _::InputUnknown;
  ASSERT_TRUE(IsNotFound(input.onMouseButton(event)));
}

TEST_F(InputTest, onScroll) {
  f32 x = 3, y = 6;
  ASSERT_TRUE(input.onScroll({x, y, -1, -1, 0}).ok());
  ASSERT_TRUE(input.mouseScrolled());
  ASSERT_TRUE(input.flags().isMouseScroll());
}

TEST_F(InputTest, reset_resetKeyboard_keyPressed) {
  KeyboardEvent event;
  event.key = KEY_A;
  event.action = ACTION_PRESS;
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_TRUE(input.isKeyPressed(event.key));
  ASSERT_TRUE(input.flags().isKeyPressed());
  ASSERT_EQ(event.key, input.keysPressed().at(0));
  input.reset();
  ASSERT_FALSE(input.isKeyPressed(event.key));
  ASSERT_FALSE(input.flags().isKeyPressed());
  ASSERT_EQ(_::InputUnknown, input.keysPressed().at(0));
}

TEST_F(InputTest, reset_resetKeyboard_keyReleased) {
  KeyboardEvent event;
  event.key = KEY_A;
  event.action = ACTION_RELEASE;
  ASSERT_TRUE(input.onKey(event).ok());
  ASSERT_TRUE(input.isKeyReleased(event.key));
  ASSERT_TRUE(input.flags().isKeyReleased());
  ASSERT_EQ(event.key, input.keysReleased().at(0));
  input.reset();
  ASSERT_FALSE(input.isKeyReleased(event.key));
  ASSERT_FALSE(input.flags().isKeyReleased());
  ASSERT_EQ(_::InputUnknown, input.keysReleased().at(0));
}

TEST_F(InputTest, reset_resetMouse_buttonPressed) {
  MouseEvent event;
  event.button = MOUSE_BUTTON_LEFT;
  event.action = ACTION_PRESS;
  ASSERT_TRUE(input.onMouseButton(event).ok());
  ASSERT_TRUE(input.isMousePressed(event.button));
  ASSERT_TRUE(input.flags().isMousePressed());
  ASSERT_EQ(event.button, input.mousePressed().at(0));
  input.reset();
  ASSERT_FALSE(input.isMousePressed(event.button));
  ASSERT_FALSE(input.flags().isMousePressed());
  ASSERT_EQ(_::InputUnknown, input.mousePressed().at(0));
}

TEST_F(InputTest, reset_resetMouse_buttonReleased) {
  MouseEvent event;
  event.button = MOUSE_BUTTON_LEFT;
  event.action = ACTION_RELEASE;
  ASSERT_TRUE(input.onMouseButton(event).ok());
  ASSERT_TRUE(input.isMouseReleased(event.button));
  ASSERT_TRUE(input.flags().isMouseReleased());
  ASSERT_EQ(event.button, input.mouseReleased().at(0));
  input.reset();
  ASSERT_FALSE(input.isMouseReleased(event.button));
  ASSERT_FALSE(input.flags().isMouseReleased());
  ASSERT_EQ(_::InputUnknown, input.mouseReleased().at(0));
}

TEST_F(InputTest, reset_resetMouse_cursorDelta) {
  ASSERT_TRUE(input.onCursorPos({3, 6, -1, -1, 0}).ok());
  ASSERT_TRUE(input.flags().isMouseMove());
  ASSERT_EQ(3, input.cursordx());
  ASSERT_EQ(6, input.cursordy());
  input.reset();
  ASSERT_FALSE(input.flags().isMouseMove());
  ASSERT_EQ(0, input.cursordx());
  ASSERT_EQ(0, input.cursordy());
}

TEST_F(InputTest, reset_resetMouse_scrollDelta) {
  ASSERT_TRUE(input.onScroll({3, 6, -1, -1, 0}).ok());
  ASSERT_TRUE(input.flags().isMouseScroll());
  ASSERT_EQ(3, input.scrolldx());
  ASSERT_EQ(6, input.scrolldy());
  input.reset();
  ASSERT_FALSE(input.flags().isMouseScroll());
  ASSERT_EQ(0, input.scrolldx());
  ASSERT_EQ(0, input.scrolldy());
}

}  // namespace uinta
