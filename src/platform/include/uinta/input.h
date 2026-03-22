#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_

#include <glm/ext/vector_float3.hpp>
#include <optional>
#include <string>

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

    [[nodiscard]] bool isKeyDown() const noexcept {
      return (flags_ & KeyDownMask) != 0;
    }

    void isKeyDown(bool v) noexcept {
      flags_ &= ~KeyDownMask;
      if (v) {
        flags_ |= KeyDownMask;
      }
    }

    [[nodiscard]] bool isKeyPressed() const noexcept {
      return (flags_ & KeyPressedMask) != 0;
    }

    void isKeyPressed(bool v) noexcept {
      flags_ &= ~KeyPressedMask;
      if (v) {
        flags_ |= KeyPressedMask;
      }
    }

    [[nodiscard]] bool isKeyReleased() const noexcept {
      return (flags_ & KeyReleasedMask) != 0;
    }

    void isKeyReleased(bool v) noexcept {
      flags_ &= ~KeyReleasedMask;
      if (v) {
        flags_ |= KeyReleasedMask;
      }
    }

    [[nodiscard]] bool isKeyRepeated() const noexcept {
      return (flags_ & KeyRepeatedMask) != 0;
    }

    void isKeyRepeated(bool v) noexcept {
      flags_ &= ~KeyRepeatedMask;
      if (v) {
        flags_ |= KeyRepeatedMask;
      }
    }

    [[nodiscard]] bool isMouseDown() const noexcept {
      return (flags_ & MouseDownMask) != 0;
    }

    void isMouseDown(bool v) noexcept {
      flags_ &= ~MouseDownMask;
      if (v) {
        flags_ |= MouseDownMask;
      }
    }

    [[nodiscard]] bool isMouseMove() const noexcept {
      return (flags_ & MouseMoveMask) != 0;
    }

    void isMouseMove(bool v) noexcept {
      flags_ &= ~MouseMoveMask;
      if (v) {
        flags_ |= MouseMoveMask;
      }
    }

    [[nodiscard]] bool isMousePressed() const noexcept {
      return (flags_ & MousePressedMask) != 0;
    }

    void isMousePressed(bool v) noexcept {
      flags_ &= ~MousePressedMask;
      if (v) {
        flags_ |= MousePressedMask;
      }
    }

    [[nodiscard]] bool isMouseReleased() const noexcept {
      return (flags_ & MouseReleasedMask) != 0;
    }

    void isMouseReleased(bool v) noexcept {
      flags_ &= ~MouseReleasedMask;
      if (v) {
        flags_ |= MouseReleasedMask;
      }
    }

    [[nodiscard]] bool isMouseScroll() const noexcept {
      return (flags_ & MouseScrollMask) != 0;
    }

    void isMouseScroll(bool v) noexcept {
      flags_ &= ~MouseScrollMask;
      if (v) {
        flags_ |= MouseScrollMask;
      }
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

  static std::optional<std::string> GetActionStr(InputKey action) noexcept {
    static std::unordered_map<InputKey, std::string> map{
        {ACTION_PRESS, "press"},
        {ACTION_RELEASE, "release"},
        {ACTION_REPEAT, "repeat"},
    };
    if (auto itr = map.find(action); itr != map.end()) {
      return itr->second;
    }
    return std::nullopt;
  }

  static std::optional<InputKey> GetKeyFromStr(
      const std::string& key) noexcept {
    static std::unordered_map<std::string, InputKey> map{
        {"SPACE", KEY_SPACE},
        {"APOSTROPHE", KEY_APOSTROPHE},
        {"COMMA", KEY_COMMA},
        {"MINUS", KEY_MINUS},
        {"PERIOD", KEY_PERIOD},
        {"SLASH", KEY_SLASH},
        {"0", KEY_0},
        {"1", KEY_1},
        {"2", KEY_2},
        {"3", KEY_3},
        {"4", KEY_4},
        {"5", KEY_5},
        {"6", KEY_6},
        {"7", KEY_7},
        {"8", KEY_8},
        {"9", KEY_9},
        {"SEMICOLON", KEY_SEMICOLON},
        {"EQUAL", KEY_EQUAL},
        {"A", KEY_A},
        {"B", KEY_B},
        {"C", KEY_C},
        {"D", KEY_D},
        {"E", KEY_E},
        {"F", KEY_F},
        {"G", KEY_G},
        {"H", KEY_H},
        {"I", KEY_I},
        {"J", KEY_J},
        {"K", KEY_K},
        {"L", KEY_L},
        {"M", KEY_M},
        {"N", KEY_N},
        {"O", KEY_O},
        {"P", KEY_P},
        {"Q", KEY_Q},
        {"R", KEY_R},
        {"S", KEY_S},
        {"T", KEY_T},
        {"U", KEY_U},
        {"V", KEY_V},
        {"W", KEY_W},
        {"X", KEY_X},
        {"Y", KEY_Y},
        {"Z", KEY_Z},
        {"LEFT_BRACKET", KEY_LEFT_BRACKET},
        {"BACKSLASH", KEY_BACKSLASH},
        {"RIGHT_BRACKET", KEY_RIGHT_BRACKET},
        {"GRAVE_ACCENT", KEY_GRAVE_ACCENT},
        {"WORLD_1", KEY_WORLD_1},
        {"WORLD_2", KEY_WORLD_2},
        {"ESCAPE", KEY_ESCAPE},
        {"ENTER", KEY_ENTER},
        {"TAB", KEY_TAB},
        {"BACKSPACE", KEY_BACKSPACE},
        {"INSERT", KEY_INSERT},
        {"DELETE", KEY_DELETE},
        {"RIGHT", KEY_RIGHT},
        {"LEFT", KEY_LEFT},
        {"DOWN", KEY_DOWN},
        {"UP", KEY_UP},
        {"PAGE_UP", KEY_PAGE_UP},
        {"PAGE_DOWN", KEY_PAGE_DOWN},
        {"HOME", KEY_HOME},
        {"END", KEY_END},
        {"CAPS_LOCK", KEY_CAPS_LOCK},
        {"SCROLL_LOCK", KEY_SCROLL_LOCK},
        {"NUM_LOCK", KEY_NUM_LOCK},
        {"PRINT_SCREEN", KEY_PRINT_SCREEN},
        {"PAUSE", KEY_PAUSE},
        {"F1", KEY_F1},
        {"F2", KEY_F2},
        {"F3", KEY_F3},
        {"F4", KEY_F4},
        {"F5", KEY_F5},
        {"F6", KEY_F6},
        {"F7", KEY_F7},
        {"F8", KEY_F8},
        {"F9", KEY_F9},
        {"F10", KEY_F10},
        {"F11", KEY_F11},
        {"F12", KEY_F12},
        {"F13", KEY_F13},
        {"F14", KEY_F14},
        {"F15", KEY_F15},
        {"F16", KEY_F16},
        {"F17", KEY_F17},
        {"F18", KEY_F18},
        {"F19", KEY_F19},
        {"F20", KEY_F20},
        {"F21", KEY_F21},
        {"F22", KEY_F22},
        {"F23", KEY_F23},
        {"F24", KEY_F24},
        {"F25", KEY_F25},
        {"KP_0", KEY_KP_0},
        {"KP_1", KEY_KP_1},
        {"KP_2", KEY_KP_2},
        {"KP_3", KEY_KP_3},
        {"KP_4", KEY_KP_4},
        {"KP_5", KEY_KP_5},
        {"KP_6", KEY_KP_6},
        {"KP_7", KEY_KP_7},
        {"KP_8", KEY_KP_8},
        {"KP_9", KEY_KP_9},
        {"KP_DECIMAL", KEY_KP_DECIMAL},
        {"KP_DIVIDE", KEY_KP_DIVIDE},
        {"KP_MULTIPLY", KEY_KP_MULTIPLY},
        {"KP_SUBTRACT", KEY_KP_SUBTRACT},
        {"KP_ADD", KEY_KP_ADD},
        {"KP_ENTER", KEY_KP_ENTER},
        {"KP_EQUAL", KEY_KP_EQUAL},
        {"LEFT_SHIFT", KEY_LEFT_SHIFT},
        {"LEFT_CONTROL", KEY_LEFT_CONTROL},
        {"LEFT_ALT", KEY_LEFT_ALT},
        {"LEFT_SUPER", KEY_LEFT_SUPER},
        {"RIGHT_SHIFT", KEY_RIGHT_SHIFT},
        {"RIGHT_CONTROL", KEY_RIGHT_CONTROL},
        {"RIGHT_ALT", KEY_RIGHT_ALT},
        {"RIGHT_SUPER", KEY_RIGHT_SUPER},
        {"MENU", KEY_MENU},
    };
    if (auto itr = map.find(key); itr != map.end()) {
      return itr->second;
    }
    return std::nullopt;
  }

  static std::optional<std::string> GetKeyStr(InputKey key) noexcept {
    static std::unordered_map<InputKey, std::string> map{
        {KEY_SPACE, "SPACE"},
        {KEY_APOSTROPHE, "APOSTROPHE"},
        {KEY_COMMA, "COMMA"},
        {KEY_MINUS, "MINUS"},
        {KEY_PERIOD, "PERIOD"},
        {KEY_SLASH, "SLASH"},
        {KEY_0, "0"},
        {KEY_1, "1"},
        {KEY_2, "2"},
        {KEY_3, "3"},
        {KEY_4, "4"},
        {KEY_5, "5"},
        {KEY_6, "6"},
        {KEY_7, "7"},
        {KEY_8, "8"},
        {KEY_9, "9"},
        {KEY_SEMICOLON, "SEMICOLON"},
        {KEY_EQUAL, "EQUAL"},
        {KEY_A, "A"},
        {KEY_B, "B"},
        {KEY_C, "C"},
        {KEY_D, "D"},
        {KEY_E, "E"},
        {KEY_F, "F"},
        {KEY_G, "G"},
        {KEY_H, "H"},
        {KEY_I, "I"},
        {KEY_J, "J"},
        {KEY_K, "K"},
        {KEY_L, "L"},
        {KEY_M, "M"},
        {KEY_N, "N"},
        {KEY_O, "O"},
        {KEY_P, "P"},
        {KEY_Q, "Q"},
        {KEY_R, "R"},
        {KEY_S, "S"},
        {KEY_T, "T"},
        {KEY_U, "U"},
        {KEY_V, "V"},
        {KEY_W, "W"},
        {KEY_X, "X"},
        {KEY_Y, "Y"},
        {KEY_Z, "Z"},
        {KEY_LEFT_BRACKET, "LEFT_BRACKET"},
        {KEY_BACKSLASH, "BACKSLASH"},
        {KEY_RIGHT_BRACKET, "RIGHT_BRACKET"},
        {KEY_GRAVE_ACCENT, "GRAVE_ACCENT"},
        {KEY_WORLD_1, "WORLD_1"},
        {KEY_WORLD_2, "WORLD_2"},
        {KEY_ESCAPE, "ESCAPE"},
        {KEY_ENTER, "ENTER"},
        {KEY_TAB, "TAB"},
        {KEY_BACKSPACE, "BACKSPACE"},
        {KEY_INSERT, "INSERT"},
        {KEY_DELETE, "DELETE"},
        {KEY_RIGHT, "RIGHT"},
        {KEY_LEFT, "LEFT"},
        {KEY_DOWN, "DOWN"},
        {KEY_UP, "UP"},
        {KEY_PAGE_UP, "PAGE_UP"},
        {KEY_PAGE_DOWN, "PAGE_DOWN"},
        {KEY_HOME, "HOME"},
        {KEY_END, "END"},
        {KEY_CAPS_LOCK, "CAPS_LOCK"},
        {KEY_SCROLL_LOCK, "SCROLL_LOCK"},
        {KEY_NUM_LOCK, "NUM_LOCK"},
        {KEY_PRINT_SCREEN, "PRINT_SCREEN"},
        {KEY_PAUSE, "PAUSE"},
        {KEY_F1, "F1"},
        {KEY_F2, "F2"},
        {KEY_F3, "F3"},
        {KEY_F4, "F4"},
        {KEY_F5, "F5"},
        {KEY_F6, "F6"},
        {KEY_F7, "F7"},
        {KEY_F8, "F8"},
        {KEY_F9, "F9"},
        {KEY_F10, "F10"},
        {KEY_F11, "F11"},
        {KEY_F12, "F12"},
        {KEY_F13, "F13"},
        {KEY_F14, "F14"},
        {KEY_F15, "F15"},
        {KEY_F16, "F16"},
        {KEY_F17, "F17"},
        {KEY_F18, "F18"},
        {KEY_F19, "F19"},
        {KEY_F20, "F20"},
        {KEY_F21, "F21"},
        {KEY_F22, "F22"},
        {KEY_F23, "F23"},
        {KEY_F24, "F24"},
        {KEY_F25, "F25"},
        {KEY_KP_0, "KP_0"},
        {KEY_KP_1, "KP_1"},
        {KEY_KP_2, "KP_2"},
        {KEY_KP_3, "KP_3"},
        {KEY_KP_4, "KP_4"},
        {KEY_KP_5, "KP_5"},
        {KEY_KP_6, "KP_6"},
        {KEY_KP_7, "KP_7"},
        {KEY_KP_8, "KP_8"},
        {KEY_KP_9, "KP_9"},
        {KEY_KP_DECIMAL, "KP_DECIMAL"},
        {KEY_KP_DIVIDE, "KP_DIVIDE"},
        {KEY_KP_MULTIPLY, "KP_MULTIPLY"},
        {KEY_KP_SUBTRACT, "KP_SUBTRACT"},
        {KEY_KP_ADD, "KP_ADD"},
        {KEY_KP_ENTER, "KP_ENTER"},
        {KEY_KP_EQUAL, "KP_EQUAL"},
        {KEY_LEFT_SHIFT, "LEFT_SHIFT"},
        {KEY_LEFT_CONTROL, "LEFT_CONTROL"},
        {KEY_LEFT_ALT, "LEFT_ALT"},
        {KEY_LEFT_SUPER, "LEFT_SUPER"},
        {KEY_RIGHT_SHIFT, "RIGHT_SHIFT"},
        {KEY_RIGHT_CONTROL, "RIGHT_CONTROL"},
        {KEY_RIGHT_ALT, "RIGHT_ALT"},
        {KEY_RIGHT_SUPER, "RIGHT_SUPER"},
        {KEY_MENU, "MENU"},
    };
    if (auto itr = map.find(key); itr != map.end()) {
      return itr->second;
    }
    return std::nullopt;
  }

  static std::optional<std::string> GetModsStr(InputMod mods) noexcept {
    if (mods == 0) {
      return std::nullopt;
    }
    std::string result;
    if ((mods & MOD_SHIFT) != 0) {
      result += "SHIFT";
    }
    if ((mods & MOD_CONTROL) != 0) {
      if (!result.empty()) {
        result += "+";
      }
      result += "CTRL";
    }
    if ((mods & MOD_ALT) != 0) {
      if (!result.empty()) {
        result += "+";
      }
      result += "ALT";
    }
    if ((mods & MOD_SUPER) != 0) {
      if (!result.empty()) {
        result += "+";
      }
      result += "SUPER";
    }
    if ((mods & MOD_CAPS_LOCK) != 0) {
      if (!result.empty()) {
        result += "+";
      }
      result += "CAPS_LOCK";
    }
    if ((mods & MOD_NUM_LOCK) != 0) {
      if (!result.empty()) {
        result += "+";
      }
      result += "NUM_LOCK";
    }
    return result;
  }

  static std::optional<MouseButton> GetMouseButtonFromStr(
      const std::string& button) noexcept {
    static std::unordered_map<std::string, MouseButton> map{
        {"LEFT", MOUSE_BUTTON_LEFT},     {"RIGHT", MOUSE_BUTTON_RIGHT},
        {"MIDDLE", MOUSE_BUTTON_MIDDLE}, {"1", MOUSE_BUTTON_1},
        {"2", MOUSE_BUTTON_2},           {"3", MOUSE_BUTTON_3},
        {"4", MOUSE_BUTTON_4},           {"5", MOUSE_BUTTON_5},
        {"6", MOUSE_BUTTON_6},           {"7", MOUSE_BUTTON_7},
        {"8", MOUSE_BUTTON_8},
    };
    if (auto itr = map.find(button); itr != map.end()) {
      return itr->second;
    }
    return std::nullopt;
  }

  static std::optional<std::string> GetMouseButtonStr(
      MouseButton button) noexcept {
    static std::unordered_map<MouseButton, std::string> map{
        {MOUSE_BUTTON_LEFT, "LEFT"},     {MOUSE_BUTTON_RIGHT, "RIGHT"},
        {MOUSE_BUTTON_MIDDLE, "MIDDLE"}, {MOUSE_BUTTON_1, "1"},
        {MOUSE_BUTTON_2, "2"},           {MOUSE_BUTTON_3, "3"},
        {MOUSE_BUTTON_4, "4"},           {MOUSE_BUTTON_5, "5"},
        {MOUSE_BUTTON_6, "6"},           {MOUSE_BUTTON_7, "7"},
        {MOUSE_BUTTON_8, "8"},
    };
    if (auto itr = map.find(button); itr != map.end()) {
      return itr->second;
    }
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

  [[nodiscard]] bool altDown() const noexcept { return (mods_ & MOD_ALT) != 0; }

  [[nodiscard]] bool anyKeyDown() const noexcept { return flags_.isKeyDown(); }

  [[nodiscard]] bool anyKeyPressed() const noexcept {
    return flags_.isKeyPressed();
  }

  [[nodiscard]] bool anyKeyReleased() const noexcept {
    return flags_.isKeyReleased();
  }

  [[nodiscard]] bool anyKeyRepeated() const noexcept {
    return flags_.isKeyRepeated();
  }

  [[nodiscard]] bool anyMouseDown() const noexcept {
    return flags_.isMouseDown();
  }

  [[nodiscard]] bool anyMousePressed() const noexcept {
    return flags_.isMousePressed();
  }

  [[nodiscard]] bool anyMouseReleased() const noexcept {
    return flags_.isMouseReleased();
  }

  [[nodiscard]] bool ctrlDown() const noexcept {
    return (mods_ & MOD_CONTROL) != 0;
  }

  [[nodiscard]] bool mouseMoved() const noexcept {
    return flags_.isMouseMove();
  }

  [[nodiscard]] bool mouseScrolled() const noexcept {
    return flags_.isMouseScroll();
  }

  [[nodiscard]] bool shiftDown() const noexcept {
    return (mods_ & MOD_SHIFT) != 0;
  }

  [[nodiscard]] bool superDown() const noexcept {
    return (mods_ & MOD_SUPER) != 0;
  }

  [[nodiscard]] f32 cursordx() const noexcept { return cursordx_; }

  [[nodiscard]] f32 cursordy() const noexcept { return cursordy_; }

  [[nodiscard]] f32 cursorx() const noexcept { return cursorx_; }

  [[nodiscard]] f32 cursory() const noexcept { return cursory_; }

  [[nodiscard]] Flags flags() const noexcept { return flags_; }

  [[nodiscard]] f32 scrolldx() const noexcept { return scrolldx_; }

  [[nodiscard]] f32 scrolldy() const noexcept { return scrolldy_; }

  [[nodiscard]] InputMod mods() const noexcept { return mods_; }

  [[nodiscard]] const KeyStorage& keysDown() const noexcept {
    return keysDown_;
  }

  [[nodiscard]] const KeyStorage& keysPressed() const noexcept {
    return keysPressed_;
  }

  [[nodiscard]] const KeyStorage& keysReleased() const noexcept {
    return keysReleased_;
  }

  [[nodiscard]] const KeyStorage& keysRepeated() const noexcept {
    return keysRepeated_;
  }

  [[nodiscard]] const MouseStorage& mouseDown() const noexcept {
    return mouseDown_;
  }

  [[nodiscard]] const MouseStorage& mousePressed() const noexcept {
    return mousePressed_;
  }

  [[nodiscard]] const MouseStorage& mouseReleased() const noexcept {
    return mouseReleased_;
  }

  [[nodiscard]] bool isKeyDown(InputKey key, InputMod mask = 0) const noexcept {
    return anyKeyDown() && (mods_ & mask) == mask &&
           containsVal(&keysDown_, key);
  }

  [[nodiscard]] bool isKeyPressed(InputKey key,
                                  InputMod mask = 0) const noexcept {
    return anyKeyPressed() && (mods_ & mask) == mask &&
           containsVal(&keysPressed_, key);
  }

  [[nodiscard]] bool isKeyReleased(InputKey key,
                                   InputMod mask = 0) const noexcept {
    return anyKeyReleased() && (mods_ & mask) == mask &&
           containsVal(&keysReleased_, key);
  }

  [[nodiscard]] bool isKeyRepeated(InputKey key,
                                   InputMod mask = 0) const noexcept {
    return anyKeyRepeated() && (mods_ & mask) == mask &&
           containsVal(&keysRepeated_, key);
  }

  [[nodiscard]] bool isMouseDown(MouseButton button,
                                 InputMod mask = 0) const noexcept {
    return anyMouseDown() && (mods_ & mask) == mask &&
           containsVal(&mouseDown_, button);
  }

  [[nodiscard]] bool isMousePressed(MouseButton button,
                                    InputMod mask = 0) const noexcept {
    return anyMousePressed() && (mods_ & mask) == mask &&
           containsVal(&mousePressed_, button);
  }

  [[nodiscard]] bool isMouseReleased(MouseButton button,
                                     InputMod mask = 0) const noexcept {
    return anyMouseReleased() && (mods_ & mask) == mask &&
           containsVal(&mouseReleased_, button);
  }

  Status onCursorPos(const MouseEvent& event) noexcept {
    cursordx_ = event.x - cursorx_;
    cursordy_ = event.y - cursory_;
    cursorx_ = event.x;
    cursory_ = event.y;
    flags_.isMouseMove(true);
    return OkStatus();
  }

  Status onKey(const KeyboardEvent& event) noexcept {
    mods_ = event.mods;
    if (event.action == ACTION_PRESS) {
      return keyPressed(event);
    }
    if (event.action == ACTION_RELEASE) {
      return keyReleased(event);
    }
    if (event.action == ACTION_REPEAT) {
      return keyRepeated(event);
    }
    return NotFoundError("KeyboardEvent::action not found.");
  }

  Status onMouseButton(const MouseEvent& event) noexcept {
    mods_ = event.mods;
    if (event.action == ACTION_PRESS) {
      return mousePressed(event);
    }
    if (event.action == ACTION_RELEASE) {
      return mouseReleased(event);
    }
    return NotFoundError("MouseEvent::action not found.");
  }

  Status onScroll(const MouseEvent& event) noexcept {
    scrolldx_ = event.x;
    scrolldy_ = event.y;
    flags_.isMouseScroll(true);
    return OkStatus();
  }

  void resetKeyboard() noexcept {
    if (flags_.isKeyPressed()) {
      flags_.isKeyPressed(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (keysPressed_.at(i) == _::InputUnknown) {
          break;
        }
        keysPressed_.at(i) = _::InputUnknown;
      }
    }

    if (flags_.isKeyReleased()) {
      flags_.isKeyReleased(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (keysReleased_.at(i) == _::InputUnknown) {
          break;
        }
        keysReleased_.at(i) = _::InputUnknown;
      }
    }
  }

  void resetMouse() noexcept {
    if (flags_.isMousePressed()) {
      flags_.isMousePressed(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (mousePressed_.at(i) == _::InputUnknown) {
          break;
        }
        mousePressed_.at(i) = _::InputUnknown;
      }
    }

    if (flags_.isMouseReleased()) {
      flags_.isMouseReleased(false);
      for (size_t i = 0; i < _::InputStorageSize; i++) {
        if (mouseReleased_.at(i) == _::InputUnknown) {
          break;
        }
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

  void reset() noexcept {
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

  Status keyPressed(const KeyboardEvent& event) noexcept {
    size_t i;
    for (i = 0; i < _::InputStorageSize; i++) {
      if (keysPressed_.at(i) == event.key) {
        break;
      }
      if (keysPressed_.at(i) == _::InputUnknown) {
        keysPressed_.at(i) = event.key;
        break;
      }
    }
    if (i == _::InputStorageSize) {
      return ResourceExhaustedError("Keys pressed storage is full");
    }
    for (i = 0; i < _::InputStorageSize; i++) {
      if (keysDown_.at(i) == event.key) {
        break;
      }
      if (keysDown_.at(i) == _::InputUnknown) {
        keysDown_.at(i) = event.key;
        break;
      }
    }
    if (i == _::InputStorageSize) {
      return ResourceExhaustedError("Keys down storage is full");
    }
    flags_.isKeyPressed(true);
    flags_.isKeyDown(true);
    return OkStatus();
  }

  Status keyRepeated(const KeyboardEvent& event) noexcept {
    size_t i;
    if (containsVal(&keysRepeated_, event.key)) {
      return OkStatus();
    }
    for (i = 0; i < _::InputStorageSize; i++) {
      if (keysRepeated_.at(i) == _::InputUnknown) {
        keysRepeated_.at(i) = event.key;
        break;
      }
    }
    if (i == _::InputStorageSize) {
      return ResourceExhaustedError("Keys repeated storage is full");
    }
    flags_.isKeyRepeated(true);
    return OkStatus();
  }

  Status keyReleased(const KeyboardEvent& event) noexcept {
    size_t i;
    for (i = 0; i < _::InputStorageSize; i++) {
      if (keysReleased_.at(i) == event.key) {
        break;
      }
      if (keysReleased_.at(i) == _::InputUnknown) {
        keysReleased_.at(i) = event.key;
        break;
      }
    }
    if (i == _::InputStorageSize) {
      return ResourceExhaustedError("Keys released storage is full");
    }
    if (removeAndCompact(&keysDown_, event.key)) {
      flags_.isKeyDown(false);
    }
    if (removeAndCompact(&keysRepeated_, event.key)) {
      flags_.isKeyRepeated(false);
    }
    flags_.isKeyReleased(true);
    return OkStatus();
  }

  Status mousePressed(const MouseEvent& event) noexcept {
    size_t i;
    for (i = 0; i < _::InputStorageSize; i++) {
      if (mousePressed_.at(i) == _::InputUnknown) {
        mousePressed_.at(i) = event.button;
        break;
      }
    }
    if (i == _::InputStorageSize) {
      return ResourceExhaustedError("Mouse pressed storage is full");
    }
    for (i = 0; i < _::InputStorageSize; i++) {
      if (mouseDown_.at(i) == _::InputUnknown) {
        mouseDown_.at(i) = event.button;
        break;
      }
    }
    if (i == _::InputStorageSize) {
      return ResourceExhaustedError("Mouse down storage is full");
    }
    flags_.isMousePressed(true);
    flags_.isMouseDown(true);
    return OkStatus();
  }

  Status mouseReleased(const MouseEvent& event) noexcept {
    size_t i;
    for (i = 0; i < _::InputStorageSize; i++) {
      if (mouseReleased_.at(i) == _::InputUnknown) {
        mouseReleased_.at(i) = event.button;
        break;
      }
    }
    if (i == _::InputStorageSize) {
      return ResourceExhaustedError("Mouse released storage is full");
    }
    if (removeAndCompact(&mouseDown_, event.button)) {
      flags_.isMouseDown(false);
    }
    flags_.isMouseReleased(true);
    return OkStatus();
  }

  Status mouseMoved(const MouseEvent& event) noexcept {
    cursordx_ = event.x - cursorx_;
    cursordy_ = event.y - cursory_;
    cursorx_ = event.x;
    cursory_ = event.y;
    flags_.isMouseMove(true);
    return OkStatus();
  }

  static bool removeAndCompact(KeyStorage* collection, InputKey v) {
    for (size_t i = 0; i < _::InputStorageSize; i++) {
      if (collection->at(i) == _::InputUnknown) {
        return i == 0;
      }
      if (collection->at(i) == v) {
        collection->at(i) = _::InputUnknown;
        for (size_t ii = i + 1; ii < _::InputStorageSize; ii++) {
          if (collection->at(ii) == _::InputUnknown) {
            return ii == 1;
          }
          collection->at(i) = collection->at(ii);
          collection->at(ii) = _::InputUnknown;
          i++;
        }
      }
    }
    return false;
  }

  static bool containsVal(const KeyStorage* arr, InputKey val) noexcept {
    const auto* const itr = std::ranges::find_if(
        arr->begin(), arr->end(),
        [val](auto v) { return v == _::InputUnknown || v == val; });
    return itr != arr->end() && *itr == val;
  }
};

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_
