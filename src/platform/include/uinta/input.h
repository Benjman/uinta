#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_

#include <algorithm>
#include <array>
#include <string>

#include "uinta/lib/absl/status.h"
#include "uinta/types.h"

namespace uinta {

using InputKey = i32;
using InputMod = i32;
using MouseButton = i32;

extern InputKey KEY_SPACE;
extern InputKey KEY_APOSTROPHE;
extern InputKey KEY_COMMA;
extern InputKey KEY_MINUS;
extern InputKey KEY_PERIOD;
extern InputKey KEY_SLASH;
extern InputKey KEY_0;
extern InputKey KEY_1;
extern InputKey KEY_2;
extern InputKey KEY_3;
extern InputKey KEY_4;
extern InputKey KEY_5;
extern InputKey KEY_6;
extern InputKey KEY_7;
extern InputKey KEY_8;
extern InputKey KEY_9;
extern InputKey KEY_SEMICOLON;
extern InputKey KEY_EQUAL;
extern InputKey KEY_A;
extern InputKey KEY_B;
extern InputKey KEY_C;
extern InputKey KEY_D;
extern InputKey KEY_E;
extern InputKey KEY_F;
extern InputKey KEY_G;
extern InputKey KEY_H;
extern InputKey KEY_I;
extern InputKey KEY_J;
extern InputKey KEY_K;
extern InputKey KEY_L;
extern InputKey KEY_M;
extern InputKey KEY_N;
extern InputKey KEY_O;
extern InputKey KEY_P;
extern InputKey KEY_Q;
extern InputKey KEY_R;
extern InputKey KEY_S;
extern InputKey KEY_T;
extern InputKey KEY_U;
extern InputKey KEY_V;
extern InputKey KEY_W;
extern InputKey KEY_X;
extern InputKey KEY_Y;
extern InputKey KEY_Z;
extern InputKey KEY_LEFT_BRACKET;
extern InputKey KEY_BACKSLASH;
extern InputKey KEY_RIGHT_BRACKET;
extern InputKey KEY_GRAVE_ACCENT;
extern InputKey KEY_WORLD_1;
extern InputKey KEY_WORLD_2;
extern InputKey KEY_ESCAPE;
extern InputKey KEY_ENTER;
extern InputKey KEY_TAB;
extern InputKey KEY_BACKSPACE;
extern InputKey KEY_INSERT;
extern InputKey KEY_DELETE;
extern InputKey KEY_RIGHT;
extern InputKey KEY_LEFT;
extern InputKey KEY_DOWN;
extern InputKey KEY_UP;
extern InputKey KEY_PAGE_UP;
extern InputKey KEY_PAGE_DOWN;
extern InputKey KEY_HOME;
extern InputKey KEY_END;
extern InputKey KEY_CAPS_LOCK;
extern InputKey KEY_SCROLL_LOCK;
extern InputKey KEY_NUM_LOCK;
extern InputKey KEY_PRINT_SCREEN;
extern InputKey KEY_PAUSE;
extern InputKey KEY_F1;
extern InputKey KEY_F2;
extern InputKey KEY_F3;
extern InputKey KEY_F4;
extern InputKey KEY_F5;
extern InputKey KEY_F6;
extern InputKey KEY_F7;
extern InputKey KEY_F8;
extern InputKey KEY_F9;
extern InputKey KEY_F10;
extern InputKey KEY_F11;
extern InputKey KEY_F12;
extern InputKey KEY_F13;
extern InputKey KEY_F14;
extern InputKey KEY_F15;
extern InputKey KEY_F16;
extern InputKey KEY_F17;
extern InputKey KEY_F18;
extern InputKey KEY_F19;
extern InputKey KEY_F20;
extern InputKey KEY_F21;
extern InputKey KEY_F22;
extern InputKey KEY_F23;
extern InputKey KEY_F24;
extern InputKey KEY_F25;
extern InputKey KEY_KP_0;
extern InputKey KEY_KP_1;
extern InputKey KEY_KP_2;
extern InputKey KEY_KP_3;
extern InputKey KEY_KP_4;
extern InputKey KEY_KP_5;
extern InputKey KEY_KP_6;
extern InputKey KEY_KP_7;
extern InputKey KEY_KP_8;
extern InputKey KEY_KP_9;
extern InputKey KEY_KP_DECIMAL;
extern InputKey KEY_KP_DIVIDE;
extern InputKey KEY_KP_MULTIPLY;
extern InputKey KEY_KP_SUBTRACT;
extern InputKey KEY_KP_ADD;
extern InputKey KEY_KP_ENTER;
extern InputKey KEY_KP_EQUAL;
extern InputKey KEY_LEFT_SHIFT;
extern InputKey KEY_LEFT_CONTROL;
extern InputKey KEY_LEFT_ALT;
extern InputKey KEY_LEFT_SUPER;
extern InputKey KEY_RIGHT_SHIFT;
extern InputKey KEY_RIGHT_CONTROL;
extern InputKey KEY_RIGHT_ALT;
extern InputKey KEY_RIGHT_SUPER;
extern InputKey KEY_MENU;
extern InputKey ACTION_PRESS;
extern InputKey ACTION_RELEASE;
extern InputKey ACTION_REPEAT;
extern InputKey MOD_SHIFT;
extern InputKey MOD_CONTROL;
extern InputKey MOD_ALT;
extern InputKey MOD_SUPER;
extern InputKey MOD_CAPS_LOCK;
extern InputKey MOD_NUM_LOCK;
extern MouseButton MOUSE_BUTTON_1;
extern MouseButton MOUSE_BUTTON_2;
extern MouseButton MOUSE_BUTTON_3;
extern MouseButton MOUSE_BUTTON_4;
extern MouseButton MOUSE_BUTTON_5;
extern MouseButton MOUSE_BUTTON_6;
extern MouseButton MOUSE_BUTTON_7;
extern MouseButton MOUSE_BUTTON_8;
extern MouseButton MOUSE_BUTTON_LEFT;
extern MouseButton MOUSE_BUTTON_RIGHT;
extern MouseButton MOUSE_BUTTON_MIDDLE;

namespace _ {
constexpr InputKey InputUnknown = -1;
constexpr size_t InputStorageSize = 8;
}  // namespace _

struct KeyboardEvent {
  const void* window;
  InputKey key;
  i32 scancode;
  i32 action;
  InputMod mods;
};

struct MouseEvent {
  const void* window;
  f64 x;
  f32 y;
  MouseButton button;
  i32 action;
  InputMod mods;
};

class Input final {
  using InputFlags = u16;
  using KeyStorage = std::array<InputKey, _::InputStorageSize>;
  using MouseStorage = std::array<MouseButton, _::InputStorageSize>;

 public:
  static constexpr InputFlags KeyDownMask = 1 << 0;
  static constexpr InputFlags KeyPressedMask = 1 << 1;
  static constexpr InputFlags KeyReleasedMask = 1 << 2;
  static constexpr InputFlags KeyRepeatedMask = 1 << 3;
  static constexpr InputFlags MouseDownMask = 1 << 4;
  static constexpr InputFlags MouseMoveMask = 1 << 5;
  static constexpr InputFlags MousePressedMask = 1 << 6;
  static constexpr InputFlags MouseReleasedMask = 1 << 7;
  static constexpr InputFlags MouseScrollMask = 1 << 8;

  static const std::string GetActionStr(InputKey) noexcept;
  static const std::string GetKeyStr(InputKey) noexcept;
  static const std::string GetModsStr(InputKey) noexcept;
  static const std::string GetMouseButtonStr(MouseButton) noexcept;

  Input() noexcept {
    keysDown_.fill(_::InputUnknown);
    keysPressed_.fill(_::InputUnknown);
    keysReleased_.fill(_::InputUnknown);
    keysRepeated_.fill(_::InputUnknown);
    mouseDown_.fill(_::InputUnknown);
    mousePressed_.fill(_::InputUnknown);
    mouseReleased_.fill(_::InputUnknown);
  }

  ~Input() noexcept = default;
  Input(const Input&) noexcept = delete;
  Input& operator=(const Input&) noexcept = delete;
  Input(Input&&) noexcept = delete;
  Input& operator=(Input&&) noexcept = delete;

  inline bool altDown() const noexcept { return mods_ & MOD_ALT; }

  inline bool anyKeyDown() const noexcept { return flags_ & KeyDownMask; }

  inline bool anyKeyPressed() const noexcept { return flags_ & KeyPressedMask; }

  inline bool anyKeyReleased() const noexcept {
    return flags_ & KeyReleasedMask;
  }

  inline bool anyKeyRepeated() const noexcept {
    return flags_ & KeyRepeatedMask;
  }

  inline bool anyMouseDown() const noexcept { return flags_ & MouseDownMask; }

  inline bool anyMousePressed() const noexcept {
    return flags_ & MousePressedMask;
  }

  inline bool anyMouseReleased() const noexcept {
    return flags_ & MouseReleasedMask;
  }

  inline bool ctrlDown() const noexcept { return mods_ & MOD_CONTROL; }

  inline bool mouseMoved() const noexcept { return flags_ & MouseMoveMask; }

  inline bool mouseScrolled() const noexcept {
    return flags_ & MouseScrollMask;
  }

  inline bool shiftDown() const noexcept { return mods_ & MOD_SHIFT; }

  inline bool superDown() const noexcept { return mods_ & MOD_SUPER; }

  inline f32 cursordx() const noexcept { return cursordx_; }

  inline f32 cursordy() const noexcept { return cursordy_; }

  inline f32 cursorx() const noexcept { return cursorx_; }

  inline f32 cursory() const noexcept { return cursory_; }

  inline f32 scrolldx() const noexcept { return scrolldx_; }

  inline f32 scrolldy() const noexcept { return scrolldy_; }

  inline InputFlags flags() const noexcept { return flags_; }

  inline InputMod mods() const noexcept { return mods_; }

  inline const KeyStorage& keysDown() const noexcept { return keysDown_; }

  inline const KeyStorage& keysPressed() const noexcept { return keysPressed_; }

  inline const KeyStorage& keysReleased() const noexcept {
    return keysReleased_;
  }

  inline const KeyStorage& keysRepeated() const noexcept {
    return keysRepeated_;
  }

  inline const MouseStorage& mouseDown() const noexcept { return mouseDown_; }

  inline const MouseStorage& mousePressed() const noexcept {
    return mousePressed_;
  }

  inline const MouseStorage& mouseReleased() const noexcept {
    return mouseReleased_;
  }

  inline bool isKeyDown(InputKey key, InputMod mask = 0) const noexcept {
    return anyKeyDown() && (mods_ & mask) == mask &&
           containsVal(&keysDown_, key);
  }

  inline bool isKeyPressed(InputKey key, InputMod mask = 0) const noexcept {
    return anyKeyPressed() && (mods_ & mask) == mask &&
           containsVal(&keysPressed_, key);
  }

  inline bool isKeyReleased(InputKey key, InputMod mask = 0) const noexcept {
    return anyKeyReleased() && (mods_ & mask) == mask &&
           containsVal(&keysReleased_, key);
  }

  inline bool isKeyRepeated(InputKey key, InputMod mask = 0) const noexcept {
    return anyKeyRepeated() && (mods_ & mask) == mask &&
           containsVal(&keysRepeated_, key);
  }

  inline bool isMouseDown(MouseButton button,
                          InputMod mask = 0) const noexcept {
    return anyMouseDown() && (mods_ & mask) == mask &&
           containsVal(&mouseDown_, button);
  }

  inline bool isMousePressed(MouseButton button,
                             InputMod mask = 0) const noexcept {
    return anyMousePressed() && (mods_ & mask) == mask &&
           containsVal(&mousePressed_, button);
  }

  inline bool isMouseReleased(MouseButton button,
                              InputMod mask = 0) const noexcept {
    return anyMouseReleased() && (mods_ & mask) == mask &&
           containsVal(&mouseReleased_, button);
  }

  inline Status onCursorPos(const MouseEvent& event) noexcept {
    cursordx_ = event.x - cursorx_;
    cursordy_ = event.y - cursory_;
    cursorx_ = event.x;
    cursory_ = event.y;
    flags_ |= MouseMoveMask;
    return {};
  }

  inline Status onKey(const KeyboardEvent& event) noexcept {
    mods_ = event.mods;
    if (event.action == ACTION_PRESS) return keyPressed(event);
    if (event.action == ACTION_RELEASE) return keyReleased(event);
    if (event.action == ACTION_REPEAT) return keyRepeated(event);
    return NotFoundError("KeyboardEvent::action not found.");
  }

  inline Status onMouseButton(const MouseEvent& event) noexcept {
    mods_ = event.mods;
    if (event.action == ACTION_PRESS) return mousePressed(event);
    if (event.action == ACTION_RELEASE) return mouseReleased(event);
    return NotFoundError("MouseEvent::action not found.");
  }

  inline Status onScroll(const MouseEvent& event) noexcept {
    scrolldx_ = event.x;
    scrolldy_ = event.y;
    flags_ |= MouseScrollMask;
    return {};
  }

  inline void resetKeyboard() noexcept {
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

  inline void resetMouse() noexcept {
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

  inline void reset() noexcept {
    resetKeyboard();
    resetMouse();
  }

 private:
  KeyStorage keysDown_;
  KeyStorage keysPressed_;
  KeyStorage keysReleased_;
  KeyStorage keysRepeated_;
  MouseStorage mouseDown_;
  MouseStorage mousePressed_;
  MouseStorage mouseReleased_;
  InputMod mods_ = 0;
  f32 cursordx_ = 0, cursordy_ = 0;
  f32 cursorx_ = 0, cursory_ = 0;
  f32 scrolldx_ = 0, scrolldy_ = 0;
  InputFlags flags_ = 0;

  inline Status keyPressed(const KeyboardEvent& event) noexcept {
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

  inline Status keyRepeated(const KeyboardEvent& event) noexcept {
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

  inline Status keyReleased(const KeyboardEvent& event) noexcept {
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

  inline Status mousePressed(const MouseEvent& event) noexcept {
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

  inline Status mouseReleased(const MouseEvent& event) noexcept {
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

  inline Status mouseMoved(const MouseEvent& event) noexcept {
    cursordx_ = event.x - cursorx_;
    cursordy_ = event.y - cursory_;
    cursorx_ = event.x;
    cursory_ = event.y;
    flags_ |= MouseMoveMask;
    return {};
  }

  inline bool removeAndCompact(KeyStorage* collection, InputKey v) {
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

  inline bool containsVal(const KeyStorage* arr, InputKey val) const noexcept {
    const auto itr = std::find_if(arr->begin(), arr->end(), [val](auto v) {
      return v == _::InputUnknown || v == val;
    });
    return itr != arr->end() && *itr == val;
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_
