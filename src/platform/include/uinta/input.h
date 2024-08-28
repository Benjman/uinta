#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_

#include <algorithm>
#include <array>
#include <string>

#include "glm/ext/vector_float3.hpp"
#include "uinta/flags.h"
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
 public:
  struct Flags final {
    using value_type = u16;

    FlagsOperations(0);

    bool isKeyDown() const noexcept { return flags_ & KeyDownMask; }
    void isKeyDown(bool v) noexcept {
      flags_ &= ~KeyDownMask;
      if (v) flags_ |= KeyDownMask;
    }

    bool isKeyPressed() const noexcept { return flags_ & KeyPressedMask; }
    void isKeyPressed(bool v) noexcept {
      flags_ &= ~KeyPressedMask;
      if (v) flags_ |= KeyPressedMask;
    }

    bool isKeyReleased() const noexcept { return flags_ & KeyReleasedMask; }
    void isKeyReleased(bool v) noexcept {
      flags_ &= ~KeyReleasedMask;
      if (v) flags_ |= KeyReleasedMask;
    }

    bool isKeyRepeated() const noexcept { return flags_ & KeyRepeatedMask; }
    void isKeyRepeated(bool v) noexcept {
      flags_ &= ~KeyRepeatedMask;
      if (v) flags_ |= KeyRepeatedMask;
    }

    bool isMouseDown() const noexcept { return flags_ & MouseDownMask; }
    void isMouseDown(bool v) noexcept {
      flags_ &= ~MouseDownMask;
      if (v) flags_ |= MouseDownMask;
    }

    bool isMouseMove() const noexcept { return flags_ & MouseMoveMask; }
    void isMouseMove(bool v) noexcept {
      flags_ &= ~MouseMoveMask;
      if (v) flags_ |= MouseMoveMask;
    }

    bool isMousePressed() const noexcept { return flags_ & MousePressedMask; }
    void isMousePressed(bool v) noexcept {
      flags_ &= ~MousePressedMask;
      if (v) flags_ |= MousePressedMask;
    }

    bool isMouseReleased() const noexcept { return flags_ & MouseReleasedMask; }
    void isMouseReleased(bool v) noexcept {
      flags_ &= ~MouseReleasedMask;
      if (v) flags_ |= MouseReleasedMask;
    }

    bool isMouseScroll() const noexcept { return flags_ & MouseScrollMask; }
    void isMouseScroll(bool v) noexcept {
      flags_ &= ~MouseScrollMask;
      if (v) flags_ |= MouseScrollMask;
    }

    bool isCursorWorldPointUpdated() const noexcept {
      return flags_ & CursorWorldPointUpdatedMask;
    }
    void isCursorWorldPointUpdated(bool v) noexcept {
      flags_ &= ~CursorWorldPointUpdatedMask;
      if (v) flags_ |= CursorWorldPointUpdatedMask;
    }

   private:
    static constexpr value_type KeyDownMask = 1 << 0;
    static constexpr value_type KeyPressedMask = 1 << 1;
    static constexpr value_type KeyReleasedMask = 1 << 2;
    static constexpr value_type KeyRepeatedMask = 1 << 3;
    static constexpr value_type MouseDownMask = 1 << 4;
    static constexpr value_type MouseMoveMask = 1 << 5;
    static constexpr value_type MousePressedMask = 1 << 6;
    static constexpr value_type MouseReleasedMask = 1 << 7;
    static constexpr value_type MouseScrollMask = 1 << 8;
    static constexpr value_type CursorWorldPointUpdatedMask = 1 << 9;

    value_type flags_;
  };

  using KeyStorage = std::array<InputKey, _::InputStorageSize>;
  using MouseStorage = std::array<MouseButton, _::InputStorageSize>;

  const std::string GetActionStr(InputKey action) noexcept {
    if (action == ACTION_PRESS) return "press";
    if (action == ACTION_RELEASE) return "release";
    if (action == ACTION_REPEAT) return "repeat";
    logFatal("Unknown action");
    return "Unknown action";
  }

  const std::string GetKeyStr(InputKey key) noexcept {
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
    logFatal("Unknown key");
    return "Unknown key";
  }

  const std::string GetModsStr(InputKey) noexcept {
    logFatal("Unimplemented");
    return "Unimplemented";
  }

  const std::string GetMouseButtonStr(MouseButton button) noexcept {
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
    logFatal("Unknown button.");
    return "Unknown button";
  }

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

  inline bool anyKeyDown() const noexcept { return flags_.isKeyDown(); }

  inline bool anyKeyPressed() const noexcept { return flags_.isKeyPressed(); }

  inline bool anyKeyReleased() const noexcept { return flags_.isKeyReleased(); }

  inline bool anyKeyRepeated() const noexcept { return flags_.isKeyRepeated(); }

  inline bool anyMouseDown() const noexcept { return flags_.isMouseDown(); }

  inline bool anyMousePressed() const noexcept {
    return flags_.isMousePressed();
  }

  inline bool anyMouseReleased() const noexcept {
    return flags_.isMouseReleased();
  }

  inline bool ctrlDown() const noexcept { return mods_ & MOD_CONTROL; }

  inline bool mouseMoved() const noexcept { return flags_.isMouseMove(); }

  inline bool mouseScrolled() const noexcept { return flags_.isMouseScroll(); }

  inline bool shiftDown() const noexcept { return mods_ & MOD_SHIFT; }

  inline bool superDown() const noexcept { return mods_ & MOD_SUPER; }

  inline f32 cursordx() const noexcept { return cursordx_; }

  inline f32 cursordy() const noexcept { return cursordy_; }

  inline f32 cursorx() const noexcept { return cursorx_; }

  inline f32 cursory() const noexcept { return cursory_; }

  inline glm::vec3 cursorWorldPoint() const noexcept {
    return cursorWorldPoint_;
  }

  inline void cursorWorldPoint(glm::vec3 pos) noexcept {
    if (cursorWorldPoint_ == pos) return;
    cursorWorldPoint_ = pos;
    flags_.isCursorWorldPointUpdated(true);
  }

  inline Flags flags() const noexcept { return flags_; }

  inline f32 scrolldx() const noexcept { return scrolldx_; }

  inline f32 scrolldy() const noexcept { return scrolldy_; }

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
    flags_.isMouseMove(true);
    return OkStatus();
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
    flags_.isMouseScroll(true);
    return OkStatus();
  }

  inline void resetKeyboard() noexcept {
    if (flags_.isKeyPressed()) {
      flags_.isKeyPressed(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (keysPressed_.at(i) == _::InputUnknown) break;
        keysPressed_.at(i) = _::InputUnknown;
      }
    }

    if (flags_.isKeyReleased()) {
      flags_.isKeyReleased(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (keysReleased_.at(i) == _::InputUnknown) break;
        keysReleased_.at(i) = _::InputUnknown;
      }
    }
  }

  inline void resetMouse() noexcept {
    if (flags_.isMousePressed()) {
      flags_.isMousePressed(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (mousePressed_.at(i) == _::InputUnknown) break;
        mousePressed_.at(i) = _::InputUnknown;
      }
    }

    if (flags_.isMouseReleased()) {
      flags_.isMouseReleased(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (mouseReleased_.at(i) == _::InputUnknown) break;
        mouseReleased_.at(i) = _::InputUnknown;
      }
    }

    flags_.isMouseMove(false);
    flags_.isMouseScroll(false);
    flags_.isCursorWorldPointUpdated(false);

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
  glm::vec3 cursorWorldPoint_{0, 0, 0};
  Flags flags_;

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
    flags_.isKeyPressed(true);
    flags_.isKeyDown(true);
    return OkStatus();
  }

  inline Status keyRepeated(const KeyboardEvent& event) noexcept {
    size_t i;
    if (containsVal(&keysRepeated_, event.key)) return OkStatus();
    for (i = 0; i < _::InputStorageSize; i++) {
      if (keysRepeated_.at(i) == _::InputUnknown) {
        keysRepeated_.at(i) = event.key;
        break;
      }
    }
    if (i == _::InputStorageSize)
      return ResourceExhaustedError("Keys repeated storage is full");
    flags_.isKeyRepeated(true);
    return OkStatus();
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
    if (removeAndCompact(&keysDown_, event.key)) flags_.isKeyDown(false);
    if (removeAndCompact(&keysRepeated_, event.key))
      flags_.isKeyRepeated(false);
    flags_.isKeyReleased(true);
    return OkStatus();
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
    flags_.isMousePressed(true);
    flags_.isMouseDown(true);
    return OkStatus();
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
    if (removeAndCompact(&mouseDown_, event.button)) flags_.isMouseDown(false);
    flags_.isMouseReleased(true);
    return OkStatus();
  }

  inline Status mouseMoved(const MouseEvent& event) noexcept {
    cursordx_ = event.x - cursorx_;
    cursordy_ = event.y - cursory_;
    cursorx_ = event.x;
    cursory_ = event.y;
    flags_.isMouseMove(true);
    return OkStatus();
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
