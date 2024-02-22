#include "uinta/input.h"

namespace uinta {

bool Input::isKeyDown(InputKey key) const noexcept {
  if (!anyKeyDown()) return false;
  for (auto v : keysDown_) {
    if (v == _::InputUnknown) return false;
    if (v == key) return true;
  }
  return false;
}

inline bool containsVal(const Input::KeyStorage* arr, InputKey val) noexcept {
  for (auto v : *arr) {
    if (v == val) return true;
    if (v == _::InputUnknown) return false;
  }
  return false;
}

bool Input::isKeyPressed(InputKey key) const noexcept {
  if (!anyKeyPressed()) return false;
  return containsVal(&keysPressed_, key);
}

bool Input::isKeyReleased(InputKey key) const noexcept {
  if (!anyKeyReleased()) return false;
  return containsVal(&keysReleased_, key);
}

bool Input::isKeyRepeated(InputKey key) const noexcept {
  if (!anyKeyRepeated()) return false;
  return containsVal(&keysRepeated_, key);
}

bool Input::isMouseReleased(MouseButton button) const noexcept {
  if (!anyMouseReleased()) return false;
  return containsVal(&mouseReleased_, button);
}

bool Input::isMouseDown(MouseButton button) const noexcept {
  if (!anyMouseDown()) return false;
  return containsVal(&mouseDown_, button);
}

bool Input::isMousePressed(MouseButton button) const noexcept {
  if (!anyMousePressed()) return false;
  return containsVal(&mousePressed_, button);
}

Status Input::keyPressed(const KeyboardEvent& event) noexcept {
  size_t i;
  for (i = 0; i < _::InputStorageSize; i++) {
    if (keysPressed_.at(i) == event.key) break;
    if (keysPressed_.at(i) == _::InputUnknown) {
      keysPressed_.at(i) = event.key;
      break;
    }
  }
  if (i == _::InputStorageSize)
    return ResourceExhaustedError("Keys pressed storage is full");
  for (i = 0; i < _::InputStorageSize; i++) {
    if (keysDown_.at(i) == event.key) break;
    if (keysDown_.at(i) == _::InputUnknown) {
      keysDown_.at(i) = event.key;
      break;
    }
  }
  if (i == _::InputStorageSize)
    return ResourceExhaustedError("Keys down storage is full");
  flags_ |= KeyPressedMask | KeyDownMask;
  return {};
}

inline bool removeAndCompact(Input::KeyStorage* collection, InputKey v) {
  for (size_t i = 0; i < _::InputStorageSize; i++) {
    if (collection->at(i) == _::InputUnknown) return i == 0;
    if (collection->at(i) == v) {
      collection->at(i) = _::InputUnknown;
      for (size_t ii = i + 1; ii < _::InputStorageSize; ii++) {
        if (collection->at(ii) == _::InputUnknown) return ii == 1;
        collection->at(i) = collection->at(ii);
        collection->at(ii) = _::InputUnknown;
        i++;
      }
    }
  }
  return false;
}

Status Input::keyReleased(const KeyboardEvent& event) noexcept {
  size_t i;
  for (i = 0; i < _::InputStorageSize; i++) {
    if (keysReleased_.at(i) == event.key) break;
    if (keysReleased_.at(i) == _::InputUnknown) {
      keysReleased_.at(i) = event.key;
      break;
    }
  }
  if (i == _::InputStorageSize)
    return ResourceExhaustedError("Keys released storage is full");
  if (removeAndCompact(&keysDown_, event.key)) flags_ &= ~KeyDownMask;
  if (removeAndCompact(&keysRepeated_, event.key)) flags_ &= ~KeyRepeatedMask;
  flags_ |= KeyReleasedMask;
  return {};
}

Status Input::keyRepeated(const KeyboardEvent& event) noexcept {
  size_t i;
  if (containsVal(&keysRepeated_, event.key)) return {};
  for (i = 0; i < _::InputStorageSize; i++) {
    if (keysRepeated_.at(i) == _::InputUnknown) {
      keysRepeated_.at(i) = event.key;
      break;
    }
  }
  if (i == _::InputStorageSize)
    return ResourceExhaustedError("Keys repeated storage is full");
  flags_ |= KeyRepeatedMask;
  return {};
}

Status Input::mousePressed(const MouseEvent& event) noexcept {
  size_t i;
  for (i = 0; i < _::InputStorageSize; i++) {
    if (mousePressed_.at(i) == _::InputUnknown) {
      mousePressed_.at(i) = event.button;
      break;
    }
  }
  if (i == _::InputStorageSize)
    return ResourceExhaustedError("Mouse pressed storage is full");
  for (i = 0; i < _::InputStorageSize; i++) {
    if (mouseDown_.at(i) == _::InputUnknown) {
      mouseDown_.at(i) = event.button;
      break;
    }
  }
  if (i == _::InputStorageSize)
    return ResourceExhaustedError("Mouse down storage is full");
  flags_ |= MousePressedMask | MouseDownMask;
  return {};
}

Status Input::mouseReleased(const MouseEvent& event) noexcept {
  size_t i;
  for (i = 0; i < _::InputStorageSize; i++) {
    if (mouseReleased_.at(i) == _::InputUnknown) {
      mouseReleased_.at(i) = event.button;
      break;
    }
  }
  if (i == _::InputStorageSize)
    return ResourceExhaustedError("Mouse released storage is full");
  if (removeAndCompact(&mouseDown_, event.button)) flags_ &= ~MouseDownMask;
  flags_ |= MouseReleasedMask;
  return {};
}

Status Input::mouseMoved(const MouseEvent& event) noexcept {
  cursordx_ = event.x - cursorx_;
  cursordy_ = event.y - cursory_;
  cursorx_ = event.x;
  cursory_ = event.y;
  flags_ |= MouseMoveMask;
  return {};
}

void Input::reset() noexcept {
  resetKeyboard();
  resetMouse();
}

void Input::resetKeyboard() noexcept {
  if (flags_ & KeyPressedMask) {
    flags_ &= ~KeyPressedMask;
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      if (keysPressed_.at(i) == _::InputUnknown) break;
      keysPressed_.at(i) = _::InputUnknown;
    }
  }

  if (flags_ & KeyReleasedMask) {
    flags_ &= ~KeyReleasedMask;
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      if (keysReleased_.at(i) == _::InputUnknown) break;
      keysReleased_.at(i) = _::InputUnknown;
    }
  }
}

void Input::resetMouse() noexcept {
  if (flags_ & MousePressedMask) {
    flags_ &= ~MousePressedMask;
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      if (mousePressed_.at(i) == _::InputUnknown) break;
      mousePressed_.at(i) = _::InputUnknown;
    }
  }

  if (flags_ & MouseReleasedMask) {
    flags_ &= ~MouseReleasedMask;
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      if (mouseReleased_.at(i) == _::InputUnknown) break;
      mouseReleased_.at(i) = _::InputUnknown;
    }
  }

  flags_ &= ~(MouseMoveMask | MouseScrollMask);

  cursordx_ = 0;
  cursordy_ = 0;
  scrolldx_ = 0;
  scrolldy_ = 0;
}

const char* getKeyStr(InputKey key) noexcept {
  if (key == KEY_SPACE) return "SPACE";
  if (key == KEY_APOSTROPHE) return "APOSTROPHE";
  if (key == KEY_COMMA) return "COMMA";
  if (key == KEY_MINUS) return "MINUS";
  if (key == KEY_PERIOD) return "PERIOD";
  if (key == KEY_SLASH) return "SLASH";
  if (key == KEY_0) return "0";
  if (key == KEY_1) return "1";
  if (key == KEY_2) return "2";
  if (key == KEY_3) return "3";
  if (key == KEY_4) return "4";
  if (key == KEY_5) return "5";
  if (key == KEY_6) return "6";
  if (key == KEY_7) return "7";
  if (key == KEY_8) return "8";
  if (key == KEY_9) return "9";
  if (key == KEY_SEMICOLON) return "SEMICOLON";
  if (key == KEY_EQUAL) return "EQUAL";
  if (key == KEY_A) return "A";
  if (key == KEY_B) return "B";
  if (key == KEY_C) return "C";
  if (key == KEY_D) return "D";
  if (key == KEY_E) return "E";
  if (key == KEY_F) return "F";
  if (key == KEY_G) return "G";
  if (key == KEY_H) return "H";
  if (key == KEY_I) return "I";
  if (key == KEY_J) return "J";
  if (key == KEY_K) return "K";
  if (key == KEY_L) return "L";
  if (key == KEY_M) return "M";
  if (key == KEY_N) return "N";
  if (key == KEY_O) return "O";
  if (key == KEY_P) return "P";
  if (key == KEY_Q) return "Q";
  if (key == KEY_R) return "R";
  if (key == KEY_S) return "S";
  if (key == KEY_T) return "T";
  if (key == KEY_U) return "U";
  if (key == KEY_V) return "V";
  if (key == KEY_W) return "W";
  if (key == KEY_X) return "X";
  if (key == KEY_Y) return "Y";
  if (key == KEY_Z) return "Z";
  if (key == KEY_LEFT_BRACKET) return "LEFT_BRACKET";
  if (key == KEY_BACKSLASH) return "BACKSLASH";
  if (key == KEY_RIGHT_BRACKET) return "RIGHT_BRACKET";
  if (key == KEY_GRAVE_ACCENT) return "GRAVE_ACCENT";
  if (key == KEY_WORLD_1) return "WORLD_1";
  if (key == KEY_WORLD_2) return "WORLD_2";
  if (key == KEY_ESCAPE) return "ESCAPE";
  if (key == KEY_ENTER) return "ENTER";
  if (key == KEY_TAB) return "TAB";
  if (key == KEY_BACKSPACE) return "BACKSPACE";
  if (key == KEY_INSERT) return "INSERT";
  if (key == KEY_DELETE) return "DELETE";
  if (key == KEY_RIGHT) return "RIGHT";
  if (key == KEY_LEFT) return "LEFT";
  if (key == KEY_DOWN) return "DOWN";
  if (key == KEY_UP) return "UP";
  if (key == KEY_PAGE_UP) return "PAGE_UP";
  if (key == KEY_PAGE_DOWN) return "PAGE_DOWN";
  if (key == KEY_HOME) return "HOME";
  if (key == KEY_END) return "END";
  if (key == KEY_CAPS_LOCK) return "CAPS_LOCK";
  if (key == KEY_SCROLL_LOCK) return "SCROLL_LOCK";
  if (key == KEY_NUM_LOCK) return "NUM_LOCK";
  if (key == KEY_PRINT_SCREEN) return "PRINT_SCREEN";
  if (key == KEY_PAUSE) return "PAUSE";
  if (key == KEY_F1) return "F1";
  if (key == KEY_F2) return "F2";
  if (key == KEY_F3) return "F3";
  if (key == KEY_F4) return "F4";
  if (key == KEY_F5) return "F5";
  if (key == KEY_F6) return "F6";
  if (key == KEY_F7) return "F7";
  if (key == KEY_F8) return "F8";
  if (key == KEY_F9) return "F9";
  if (key == KEY_F10) return "F10";
  if (key == KEY_F11) return "F11";
  if (key == KEY_F12) return "F12";
  if (key == KEY_F13) return "F13";
  if (key == KEY_F14) return "F14";
  if (key == KEY_F15) return "F15";
  if (key == KEY_F16) return "F16";
  if (key == KEY_F17) return "F17";
  if (key == KEY_F18) return "F18";
  if (key == KEY_F19) return "F19";
  if (key == KEY_F20) return "F20";
  if (key == KEY_F21) return "F21";
  if (key == KEY_F22) return "F22";
  if (key == KEY_F23) return "F23";
  if (key == KEY_F24) return "F24";
  if (key == KEY_F25) return "F25";
  if (key == KEY_KP_0) return "KP_0";
  if (key == KEY_KP_1) return "KP_1";
  if (key == KEY_KP_2) return "KP_2";
  if (key == KEY_KP_3) return "KP_3";
  if (key == KEY_KP_4) return "KP_4";
  if (key == KEY_KP_5) return "KP_5";
  if (key == KEY_KP_6) return "KP_6";
  if (key == KEY_KP_7) return "KP_7";
  if (key == KEY_KP_8) return "KP_8";
  if (key == KEY_KP_9) return "KP_9";
  if (key == KEY_KP_DECIMAL) return "KP_DECIMAL";
  if (key == KEY_KP_DIVIDE) return "KP_DIVIDE";
  if (key == KEY_KP_MULTIPLY) return "KP_MULTIPLY";
  if (key == KEY_KP_SUBTRACT) return "KP_SUBTRACT";
  if (key == KEY_KP_ADD) return "KP_ADD";
  if (key == KEY_KP_ENTER) return "KP_ENTER";
  if (key == KEY_KP_EQUAL) return "KP_EQUAL";
  if (key == KEY_LEFT_SHIFT) return "LEFT_SHIFT";
  if (key == KEY_LEFT_CONTROL) return "LEFT_CONTROL";
  if (key == KEY_LEFT_ALT) return "LEFT_ALT";
  if (key == KEY_LEFT_SUPER) return "LEFT_SUPER";
  if (key == KEY_RIGHT_SHIFT) return "RIGHT_SHIFT";
  if (key == KEY_RIGHT_CONTROL) return "RIGHT_CONTROL";
  if (key == KEY_RIGHT_ALT) return "RIGHT_ALT";
  if (key == KEY_RIGHT_SUPER) return "RIGHT_SUPER";
  if (key == KEY_MENU) return "MENU";
  return "KEY_UNKNOWN";
}

const char* getActionStr(InputKey action) noexcept {
  if (action == ACTION_PRESS) return "press";
  if (action == ACTION_RELEASE) return "release";
  if (action == ACTION_REPEAT) return "repeat";
  return "ACTION UNKNOWN";
}

const char* getModsStr(InputKey mods) noexcept {
  // TODO implement mods string
  if (mods == 0) return "";
  return "NOT IMPLEMENTED";
}

const char* getMouseButtonStr(MouseButton button) noexcept {
  if (button == MOUSE_BUTTON_LEFT) return "button left";
  if (button == MOUSE_BUTTON_RIGHT) return "button right";
  if (button == MOUSE_BUTTON_MIDDLE) return "button middle";
  if (button == MOUSE_BUTTON_1) return "button 1";
  if (button == MOUSE_BUTTON_2) return "button 2";
  if (button == MOUSE_BUTTON_3) return "button 3";
  if (button == MOUSE_BUTTON_4) return "button 4";
  if (button == MOUSE_BUTTON_5) return "button 5";
  if (button == MOUSE_BUTTON_6) return "button 6";
  if (button == MOUSE_BUTTON_7) return "button 7";
  if (button == MOUSE_BUTTON_8) return "button 8";
  return "NOT IMPLEMENTED";
}

Status Input::onKey(const KeyboardEvent& event) noexcept {
  mods_ = event.mods;
  if (event.action == ACTION_PRESS) return keyPressed(event);
  if (event.action == ACTION_RELEASE) return keyReleased(event);
  if (event.action == ACTION_REPEAT) return keyRepeated(event);
  return NotFoundError("KeyboardEvent::action not found.");
}

Status Input::onCursorPos(const MouseEvent& event) noexcept {
  cursordx_ = event.x - cursorx_;
  cursordy_ = event.y - cursory_;
  cursorx_ = event.x;
  cursory_ = event.y;
  flags_ |= MouseMoveMask;
  return {};
}

Status Input::onScroll(const MouseEvent& event) noexcept {
  scrolldx_ = event.x;
  scrolldy_ = event.y;
  flags_ |= MouseScrollMask;
  return {};
}

Status Input::onMouseButton(const MouseEvent& event) noexcept {
  mods_ = event.mods;
  if (event.action == ACTION_PRESS) return mousePressed(event);
  if (event.action == ACTION_RELEASE) return mouseReleased(event);
  return NotFoundError("MouseEvent::action not found.");
}

}  // namespace uinta
