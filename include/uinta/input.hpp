#ifndef UINTA_INPUT_HPP
#define UINTA_INPUT_HPP

#include <unordered_set>

namespace uinta {

using input_key_t    = int;
using mouse_button_t = int;

const input_key_t KEY_UNKNOWN    = -1;
const input_key_t ACTION_UNKNOWN = -1;
const input_key_t MOD_UNKNOWN    = -1;

// input and mouse keys are defined per-platform. Example in glfw_runner.cpp
extern input_key_t KEY_SPACE;
extern input_key_t KEY_APOSTROPHE;
extern input_key_t KEY_COMMA;
extern input_key_t KEY_MINUS;
extern input_key_t KEY_PERIOD;
extern input_key_t KEY_SLASH;
extern input_key_t KEY_0;
extern input_key_t KEY_1;
extern input_key_t KEY_2;
extern input_key_t KEY_3;
extern input_key_t KEY_4;
extern input_key_t KEY_5;
extern input_key_t KEY_6;
extern input_key_t KEY_7;
extern input_key_t KEY_8;
extern input_key_t KEY_9;
extern input_key_t KEY_SEMICOLON;
extern input_key_t KEY_EQUAL;
extern input_key_t KEY_A;
extern input_key_t KEY_B;
extern input_key_t KEY_C;
extern input_key_t KEY_D;
extern input_key_t KEY_E;
extern input_key_t KEY_F;
extern input_key_t KEY_G;
extern input_key_t KEY_H;
extern input_key_t KEY_I;
extern input_key_t KEY_J;
extern input_key_t KEY_K;
extern input_key_t KEY_L;
extern input_key_t KEY_M;
extern input_key_t KEY_N;
extern input_key_t KEY_O;
extern input_key_t KEY_P;
extern input_key_t KEY_Q;
extern input_key_t KEY_R;
extern input_key_t KEY_S;
extern input_key_t KEY_T;
extern input_key_t KEY_U;
extern input_key_t KEY_V;
extern input_key_t KEY_W;
extern input_key_t KEY_X;
extern input_key_t KEY_Y;
extern input_key_t KEY_Z;
extern input_key_t KEY_LEFT_BRACKET;
extern input_key_t KEY_BACKSLASH;
extern input_key_t KEY_RIGHT_BRACKET;
extern input_key_t KEY_GRAVE_ACCENT;
extern input_key_t KEY_WORLD_1;
extern input_key_t KEY_WORLD_2;
extern input_key_t KEY_ESCAPE;
extern input_key_t KEY_ENTER;
extern input_key_t KEY_TAB;
extern input_key_t KEY_BACKSPACE;
extern input_key_t KEY_INSERT;
extern input_key_t KEY_DELETE;
extern input_key_t KEY_RIGHT;
extern input_key_t KEY_LEFT;
extern input_key_t KEY_DOWN;
extern input_key_t KEY_UP;
extern input_key_t KEY_PAGE_UP;
extern input_key_t KEY_PAGE_DOWN;
extern input_key_t KEY_HOME;
extern input_key_t KEY_END;
extern input_key_t KEY_CAPS_LOCK;
extern input_key_t KEY_SCROLL_LOCK;
extern input_key_t KEY_NUM_LOCK;
extern input_key_t KEY_PRINT_SCREEN;
extern input_key_t KEY_PAUSE;
extern input_key_t KEY_F1;
extern input_key_t KEY_F2;
extern input_key_t KEY_F3;
extern input_key_t KEY_F4;
extern input_key_t KEY_F5;
extern input_key_t KEY_F6;
extern input_key_t KEY_F7;
extern input_key_t KEY_F8;
extern input_key_t KEY_F9;
extern input_key_t KEY_F10;
extern input_key_t KEY_F11;
extern input_key_t KEY_F12;
extern input_key_t KEY_F13;
extern input_key_t KEY_F14;
extern input_key_t KEY_F15;
extern input_key_t KEY_F16;
extern input_key_t KEY_F17;
extern input_key_t KEY_F18;
extern input_key_t KEY_F19;
extern input_key_t KEY_F20;
extern input_key_t KEY_F21;
extern input_key_t KEY_F22;
extern input_key_t KEY_F23;
extern input_key_t KEY_F24;
extern input_key_t KEY_F25;
extern input_key_t KEY_KP_0;
extern input_key_t KEY_KP_1;
extern input_key_t KEY_KP_2;
extern input_key_t KEY_KP_3;
extern input_key_t KEY_KP_4;
extern input_key_t KEY_KP_5;
extern input_key_t KEY_KP_6;
extern input_key_t KEY_KP_7;
extern input_key_t KEY_KP_8;
extern input_key_t KEY_KP_9;
extern input_key_t KEY_KP_DECIMAL;
extern input_key_t KEY_KP_DIVIDE;
extern input_key_t KEY_KP_MULTIPLY;
extern input_key_t KEY_KP_SUBTRACT;
extern input_key_t KEY_KP_ADD;
extern input_key_t KEY_KP_ENTER;
extern input_key_t KEY_KP_EQUAL;
extern input_key_t KEY_LEFT_SHIFT;
extern input_key_t KEY_LEFT_CONTROL;
extern input_key_t KEY_LEFT_ALT;
extern input_key_t KEY_LEFT_SUPER;
extern input_key_t KEY_RIGHT_SHIFT;
extern input_key_t KEY_RIGHT_CONTROL;
extern input_key_t KEY_RIGHT_ALT;
extern input_key_t KEY_RIGHT_SUPER;
extern input_key_t KEY_MENU;
extern input_key_t ACTION_PRESS;
extern input_key_t ACTION_RELEASE;
extern input_key_t ACTION_REPEAT;
extern input_key_t MOD_SHIFT;
extern input_key_t MOD_CONTROL;
extern input_key_t MOD_ALT;
extern input_key_t MOD_SUPER;
extern input_key_t MOD_CAPS_LOCK;
extern input_key_t MOD_NUM_LOCK;
extern mouse_button_t MOUSE_BUTTON_1;
extern mouse_button_t MOUSE_BUTTON_2;
extern mouse_button_t MOUSE_BUTTON_3;
extern mouse_button_t MOUSE_BUTTON_4;
extern mouse_button_t MOUSE_BUTTON_5;
extern mouse_button_t MOUSE_BUTTON_6;
extern mouse_button_t MOUSE_BUTTON_7;
extern mouse_button_t MOUSE_BUTTON_8;
extern mouse_button_t MOUSE_BUTTON_LAST;
extern mouse_button_t MOUSE_BUTTON_LEFT;
extern mouse_button_t MOUSE_BUTTON_RIGHT;
extern mouse_button_t MOUSE_BUTTON_MIDDLE;

struct InputState final {
  int flags = 0;
  float cursorx, cursory;
  float cursordx, cursordy;
  float scrolldx, scrolldy;
  std::unordered_set<input_key_t> keys_down;
  std::unordered_set<input_key_t> keys_pressed;
  std::unordered_set<input_key_t> keys_released;
  std::unordered_set<input_key_t> keys_repeated;
  std::unordered_set<mouse_button_t> mouse_down;
  std::unordered_set<mouse_button_t> mouse_pressed;
  std::unordered_set<mouse_button_t> mouse_released;

  void reset() {
    cursordx = 0;
    cursordy = 0;
    scrolldx = 0;
    scrolldy = 0;
    keys_pressed.clear();
    keys_released.clear();
    keys_repeated.clear();
    mouse_pressed.clear();
    mouse_released.clear();
  }

  bool isAltDown() const { return flags & MOD_ALT; }
  bool isAnyKeyDown() const { return keys_down.size(); }
  bool isAnyMouseBonttonDown() const { return mouse_down.size(); }
  bool isCtrlDown() const { return flags & MOD_CONTROL; }
  bool isKeyDown(input_key_t key) const;
  bool isKeyPressed(input_key_t key) const;
  bool isKeyReleased(input_key_t key) const;
  bool isKeyRepeated(input_key_t key) const;
  bool isMouseButtonDown(mouse_button_t button) const;
  bool isMouseButtonPressed(mouse_button_t button) const;
  bool isMouseButtonReleased(mouse_button_t button) const;
  bool isShiftDown() const;
  void keyPressed(const input_key_t key, const int flags);
  void keyReleased(const input_key_t key, const int flags);
  void keyRepeated(const input_key_t key, const int flags);
  void mouseButtonPressed(const input_key_t key, const int flags);
  void mouseButtonReleased(const input_key_t key, const int flags);
};

const char *const getKeyStr(input_key_t key) noexcept;
const char *const getActionStr(input_key_t action) noexcept;
const char *const getModsStr(input_key_t mods) noexcept;
const char *const getMouseButtonStr(mouse_button_t button) noexcept;

} // namespace uinta

#endif // UINTA_INPUT_HPP
