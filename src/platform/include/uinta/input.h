#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_

#include <array>

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
extern MouseButton MOUSE_BUTTON_LAST;
extern MouseButton MOUSE_BUTTON_LEFT;
extern MouseButton MOUSE_BUTTON_RIGHT;
extern MouseButton MOUSE_BUTTON_MIDDLE;

namespace _ {
constexpr InputKey InputUnknown = -1;
constexpr size_t InputStorageSize = 8;
}  // namespace _

class InputHandler {
 public:
  struct KeyboardEvent {
    KeyboardEvent(i32 key = 0, i32 action = 0, i32 mods = 0, i32 scancode = 0,
                  const void* window = nullptr) noexcept
        : window(window),
          key(key),
          scancode(scancode),
          action(action),
          mods(mods) {}

    const void* window;
    InputKey key;
    i32 scancode;
    i32 action;
    InputMod mods;
  };

  struct MouseEvent {
    MouseEvent(f64 x = 0, f32 y = 0, i32 button = 0, i32 action = 0,
               i32 mods = 0, const void* window = nullptr) noexcept
        : window(window),
          x(x),
          y(y),
          button(button),
          action(action),
          mods(mods) {}

    const void* window;
    f64 x;
    f32 y;
    MouseButton button;
    i32 action;
    InputMod mods;
  };

  virtual Status onCursorPos(const MouseEvent&) noexcept = 0;
  virtual Status onKey(const KeyboardEvent&) noexcept = 0;
  virtual Status onMouseButton(const MouseEvent&) noexcept = 0;
  virtual Status onScroll(const MouseEvent&) noexcept = 0;
};

using KeyboardEvent = InputHandler::KeyboardEvent;
using MouseEvent = InputHandler::MouseEvent;

class Input final : public InputHandler {
 public:
  using InputFlags = bitflag16;
  using KeyStorage = std::array<InputKey, _::InputStorageSize>;
  using MouseStorage = std::array<MouseButton, _::InputStorageSize>;

  static constexpr InputFlags KeyDownMask = 1 << 0;
  static constexpr InputFlags KeyPressedMask = 1 << 1;
  static constexpr InputFlags KeyReleasedMask = 1 << 2;
  static constexpr InputFlags KeyRepeatedMask = 1 << 3;
  static constexpr InputFlags MouseDownMask = 1 << 4;
  static constexpr InputFlags MouseMoveMask = 1 << 5;
  static constexpr InputFlags MousePressedMask = 1 << 6;
  static constexpr InputFlags MouseReleasedMask = 1 << 7;
  static constexpr InputFlags MouseScrollMask = 1 << 8;

  Input() noexcept {
    keysDown_.fill(_::InputUnknown);
    keysPressed_.fill(_::InputUnknown);
    keysReleased_.fill(_::InputUnknown);
    keysRepeated_.fill(_::InputUnknown);
    mouseDown_.fill(_::InputUnknown);
    mousePressed_.fill(_::InputUnknown);
    mouseReleased_.fill(_::InputUnknown);
  }

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

  inline bool mouseScrolled() const noexcept {
    return flags_ & MouseScrollMask;
  }

  inline bool mouseMoved() const noexcept { return flags_ & MouseMoveMask; }

  inline bool shiftDown() const noexcept { return mods_ & MOD_SHIFT; }

  inline bool superDown() const noexcept { return mods_ & MOD_SUPER; }

  inline f32 cursorx() const noexcept { return cursorx_; }
  inline f32 cursordx() const noexcept { return cursordx_; }
  inline f32 cursory() const noexcept { return cursory_; }
  inline f32 cursordy() const noexcept { return cursordy_; }
  inline f32 scrolldx() const noexcept { return scrolldx_; }
  inline f32 scrolldy() const noexcept { return scrolldy_; }
  inline InputFlags flags() const noexcept { return flags_; }
  inline InputMod mods() const noexcept { return mods_; }

  const KeyStorage& keysDown() const noexcept { return keysDown_; }
  const KeyStorage& keysPressed() const noexcept { return keysPressed_; }
  const KeyStorage& keysReleased() const noexcept { return keysReleased_; }
  const KeyStorage& keysRepeated() const noexcept { return keysRepeated_; }
  const MouseStorage& mouseDown() const noexcept { return mouseDown_; }
  const MouseStorage& mousePressed() const noexcept { return mousePressed_; }
  const MouseStorage& mouseReleased() const noexcept { return mouseReleased_; }

  bool isKeyDown(InputKey) const noexcept;
  bool isKeyPressed(InputKey) const noexcept;
  bool isKeyReleased(InputKey) const noexcept;
  bool isKeyRepeated(InputKey) const noexcept;
  bool isMouseDown(MouseButton) const noexcept;
  bool isMousePressed(MouseButton) const noexcept;
  bool isMouseReleased(MouseButton) const noexcept;

  Status onCursorPos(const MouseEvent&) noexcept override;
  Status onKey(const KeyboardEvent&) noexcept override;
  Status onMouseButton(const MouseEvent&) noexcept override;
  Status onScroll(const MouseEvent&) noexcept override;

  void reset() noexcept;

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

  Status keyPressed(const KeyboardEvent&) noexcept;
  Status keyRepeated(const KeyboardEvent&) noexcept;
  Status keyReleased(const KeyboardEvent&) noexcept;
  Status mousePressed(const MouseEvent&) noexcept;
  Status mouseRepeated(const MouseEvent&) noexcept;
  Status mouseReleased(const MouseEvent&) noexcept;
  Status mouseMoved(const MouseEvent&) noexcept;

  void resetKeyboard() noexcept;
  void resetMouse() noexcept;
};

const char* getActionStr(InputKey) noexcept;
const char* getKeyStr(InputKey) noexcept;
const char* getModsStr(InputKey) noexcept;
const char* getMouseButtonStr(MouseButton) noexcept;

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_INPUT_H_
