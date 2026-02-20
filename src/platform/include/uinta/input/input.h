#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_H_

#include <algorithm>
#include <optional>
#include <string>

#include "glm/ext/vector_float3.hpp"
#include "uinta/flags.h"
#include "uinta/input/fwd.h"
#include "uinta/status.h"
#include "uinta/types.h"

namespace uinta {

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

    value_type flags_;
  };

  using KeyStorage = std::array<InputKey, _::InputStorageSize>;
  using MouseStorage = std::array<MouseButton, _::InputStorageSize>;

  static const std::optional<std::string> GetActionStr(
      InputKey action) noexcept {
    if (action == ACTION_PRESS) return "press";
    if (action == ACTION_RELEASE) return "release";
    if (action == ACTION_REPEAT) return "repeat";
    return std::nullopt;
  }

  static std::optional<InputKey> GetKeyFromStr(std::string key) noexcept {
    if (key == "SPACE") return KEY_SPACE;
    if (key == "APOSTROPHE") return KEY_APOSTROPHE;
    if (key == "COMMA") return KEY_COMMA;
    if (key == "MINUS") return KEY_MINUS;
    if (key == "PERIOD") return KEY_PERIOD;
    if (key == "SLASH") return KEY_SLASH;
    if (key == "0") return KEY_0;
    if (key == "1") return KEY_1;
    if (key == "2") return KEY_2;
    if (key == "3") return KEY_3;
    if (key == "4") return KEY_4;
    if (key == "5") return KEY_5;
    if (key == "6") return KEY_6;
    if (key == "7") return KEY_7;
    if (key == "8") return KEY_8;
    if (key == "9") return KEY_9;
    if (key == "SEMICOLON") return KEY_SEMICOLON;
    if (key == "EQUAL") return KEY_EQUAL;
    if (key == "A") return KEY_A;
    if (key == "B") return KEY_B;
    if (key == "C") return KEY_C;
    if (key == "D") return KEY_D;
    if (key == "E") return KEY_E;
    if (key == "F") return KEY_F;
    if (key == "G") return KEY_G;
    if (key == "H") return KEY_H;
    if (key == "I") return KEY_I;
    if (key == "J") return KEY_J;
    if (key == "K") return KEY_K;
    if (key == "L") return KEY_L;
    if (key == "M") return KEY_M;
    if (key == "N") return KEY_N;
    if (key == "O") return KEY_O;
    if (key == "P") return KEY_P;
    if (key == "Q") return KEY_Q;
    if (key == "R") return KEY_R;
    if (key == "S") return KEY_S;
    if (key == "T") return KEY_T;
    if (key == "U") return KEY_U;
    if (key == "V") return KEY_V;
    if (key == "W") return KEY_W;
    if (key == "X") return KEY_X;
    if (key == "Y") return KEY_Y;
    if (key == "Z") return KEY_Z;
    if (key == "LEFT_BRACKET") return KEY_LEFT_BRACKET;
    if (key == "BACKSLASH") return KEY_BACKSLASH;
    if (key == "RIGHT_BRACKET") return KEY_RIGHT_BRACKET;
    if (key == "GRAVE_ACCENT") return KEY_GRAVE_ACCENT;
    if (key == "WORLD_1") return KEY_WORLD_1;
    if (key == "WORLD_2") return KEY_WORLD_2;
    if (key == "ESCAPE") return KEY_ESCAPE;
    if (key == "ENTER") return KEY_ENTER;
    if (key == "TAB") return KEY_TAB;
    if (key == "BACKSPACE") return KEY_BACKSPACE;
    if (key == "INSERT") return KEY_INSERT;
    if (key == "DELETE") return KEY_DELETE;
    if (key == "RIGHT") return KEY_RIGHT;
    if (key == "LEFT") return KEY_LEFT;
    if (key == "DOWN") return KEY_DOWN;
    if (key == "UP") return KEY_UP;
    if (key == "PAGE_UP") return KEY_PAGE_UP;
    if (key == "PAGE_DOWN") return KEY_PAGE_DOWN;
    if (key == "HOME") return KEY_HOME;
    if (key == "END") return KEY_END;
    if (key == "CAPS_LOCK") return KEY_CAPS_LOCK;
    if (key == "SCROLL_LOCK") return KEY_SCROLL_LOCK;
    if (key == "NUM_LOCK") return KEY_NUM_LOCK;
    if (key == "PRINT_SCREEN") return KEY_PRINT_SCREEN;
    if (key == "PAUSE") return KEY_PAUSE;
    if (key == "F1") return KEY_F1;
    if (key == "F2") return KEY_F2;
    if (key == "F3") return KEY_F3;
    if (key == "F4") return KEY_F4;
    if (key == "F5") return KEY_F5;
    if (key == "F6") return KEY_F6;
    if (key == "F7") return KEY_F7;
    if (key == "F8") return KEY_F8;
    if (key == "F9") return KEY_F9;
    if (key == "F10") return KEY_F10;
    if (key == "F11") return KEY_F11;
    if (key == "F12") return KEY_F12;
    if (key == "F13") return KEY_F13;
    if (key == "F14") return KEY_F14;
    if (key == "F15") return KEY_F15;
    if (key == "F16") return KEY_F16;
    if (key == "F17") return KEY_F17;
    if (key == "F18") return KEY_F18;
    if (key == "F19") return KEY_F19;
    if (key == "F20") return KEY_F20;
    if (key == "F21") return KEY_F21;
    if (key == "F22") return KEY_F22;
    if (key == "F23") return KEY_F23;
    if (key == "F24") return KEY_F24;
    if (key == "F25") return KEY_F25;
    if (key == "KP_0") return KEY_KP_0;
    if (key == "KP_1") return KEY_KP_1;
    if (key == "KP_2") return KEY_KP_2;
    if (key == "KP_3") return KEY_KP_3;
    if (key == "KP_4") return KEY_KP_4;
    if (key == "KP_5") return KEY_KP_5;
    if (key == "KP_6") return KEY_KP_6;
    if (key == "KP_7") return KEY_KP_7;
    if (key == "KP_8") return KEY_KP_8;
    if (key == "KP_9") return KEY_KP_9;
    if (key == "KP_DECIMAL") return KEY_KP_DECIMAL;
    if (key == "KP_DIVIDE") return KEY_KP_DIVIDE;
    if (key == "KP_MULTIPLY") return KEY_KP_MULTIPLY;
    if (key == "KP_SUBTRACT") return KEY_KP_SUBTRACT;
    if (key == "KP_ADD") return KEY_KP_ADD;
    if (key == "KP_ENTER") return KEY_KP_ENTER;
    if (key == "KP_EQUAL") return KEY_KP_EQUAL;
    if (key == "LEFT_SHIFT") return KEY_LEFT_SHIFT;
    if (key == "LEFT_CONTROL") return KEY_LEFT_CONTROL;
    if (key == "LEFT_ALT") return KEY_LEFT_ALT;
    if (key == "LEFT_SUPER") return KEY_LEFT_SUPER;
    if (key == "RIGHT_SHIFT") return KEY_RIGHT_SHIFT;
    if (key == "RIGHT_CONTROL") return KEY_RIGHT_CONTROL;
    if (key == "RIGHT_ALT") return KEY_RIGHT_ALT;
    if (key == "RIGHT_SUPER") return KEY_RIGHT_SUPER;
    if (key == "MENU") return KEY_MENU;
    return std::nullopt;
  }

  static const std::optional<std::string> GetKeyStr(InputKey key) noexcept {
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
    return std::nullopt;
  }

  static const std::optional<std::string> GetModsStr(InputKey) noexcept {
    return std::nullopt;
  }

  static std::optional<MouseButton> GetMouseButtonFromStr(
      std::string button) noexcept {
    if (button == "LEFT") return MOUSE_BUTTON_LEFT;
    if (button == "RIGHT") return MOUSE_BUTTON_RIGHT;
    if (button == "MIDDLE") return MOUSE_BUTTON_MIDDLE;
    if (button == "1") return MOUSE_BUTTON_1;
    if (button == "2") return MOUSE_BUTTON_2;
    if (button == "3") return MOUSE_BUTTON_3;
    if (button == "4") return MOUSE_BUTTON_4;
    if (button == "5") return MOUSE_BUTTON_5;
    if (button == "6") return MOUSE_BUTTON_6;
    if (button == "7") return MOUSE_BUTTON_7;
    if (button == "8") return MOUSE_BUTTON_8;
    return std::nullopt;
  }

  static const std::optional<std::string> GetMouseButtonStr(
      MouseButton button) noexcept {
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
    return std::nullopt;
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

#endif  // SRC_PLATFORM_INCLUDE_UINTA_INPUT_INPUT_H_
