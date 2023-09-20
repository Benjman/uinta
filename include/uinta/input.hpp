#ifndef UINTA_INPUT_HPP
#define UINTA_INPUT_HPP

#include <algorithm>
#include <uinta/fwd.hpp>
#include <unordered_set>

namespace uinta {

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

constexpr input_key_t KEY_UNKNOWN = -1;
constexpr input_key_t ACTION_UNKNOWN = -1;
constexpr input_key_t MOD_UNKNOWN = -1;

namespace input {

static constexpr flag_t HAS_KEY_DOWN = 1 << 0;
static constexpr flag_t HAS_KEY_PRESSED = 1 << 1;
static constexpr flag_t HAS_KEY_RELEASED = 1 << 2;
static constexpr flag_t HAS_KEY_REPEATED = 1 << 3;
static constexpr flag_t HAS_MOUSE_DOWN = 1 << 4;
static constexpr flag_t HAS_MOUSE_MOVE = 1 << 5;
static constexpr flag_t HAS_MOUSE_PRESSED = 1 << 6;
static constexpr flag_t HAS_MOUSE_RELEASED = 1 << 7;
static constexpr flag_t HAS_MOUSE_SCROLL = 1 << 8;

}  // namespace input

struct InputState final {
  i32 platform_flags = 0;
  flags_t flags = 0;
  f32 cursorx, cursory;
  f32 cursordx, cursordy;
  f32 scrolldx, scrolldy;
  std::unordered_set<input_key_t> keys_down;
  std::unordered_set<input_key_t> keys_pressed;
  std::unordered_set<input_key_t> keys_released;
  std::unordered_set<input_key_t> keys_repeated;
  std::unordered_set<mouse_button_t> mouse_down;
  std::unordered_set<mouse_button_t> mouse_pressed;
  std::unordered_set<mouse_button_t> mouse_released;
};

inline bool isAnyKeyDown(const InputState& state) {
  return isFlagSet(input::HAS_KEY_DOWN, state.flags);
}

inline bool isAnyKeyPressed(const InputState& state) {
  return isFlagSet(input::HAS_KEY_PRESSED, state.flags);
}

inline bool isAnyKeyReleased(const InputState& state) {
  return isFlagSet(input::HAS_KEY_RELEASED, state.flags);
}

inline bool isAnyKeyRepeated(const InputState& state) {
  return isFlagSet(input::HAS_KEY_REPEATED, state.flags);
}

inline bool isAnyMouseButtonDown(const InputState& state) {
  return isFlagSet(input::HAS_MOUSE_DOWN, state.flags);
}

inline bool isAnyMouseDown(const InputState& state) {
  return isFlagSet(input::HAS_MOUSE_DOWN, state.flags);
}

inline bool isAnyMouseMove(const InputState& state) {
  return isFlagSet(input::HAS_MOUSE_MOVE, state.flags);
}

inline bool isAnyMousePressed(const InputState& state) {
  return isFlagSet(input::HAS_MOUSE_PRESSED, state.flags);
}

inline bool isAnyMouseReleased(const InputState& state) {
  return isFlagSet(input::HAS_MOUSE_RELEASED, state.flags);
}

inline bool isAltDown(const InputState& state) {
  return isAnyKeyDown(state) && state.platform_flags & MOD_ALT;
}

inline bool isCtrlDown(const InputState& state) {
  return isAnyKeyDown(state) && state.platform_flags & MOD_CONTROL;
}

inline bool isKeyDown(const InputState& state, input_key_t key) {
  return isAnyKeyDown(state) && std::find(state.keys_down.begin(), state.keys_down.end(), key) != state.keys_down.end();
}

inline bool isKeyPressed(const InputState& state, input_key_t key) {
  return isAnyKeyPressed(state) &&
         std::find(state.keys_pressed.begin(), state.keys_pressed.end(), key) != state.keys_pressed.end();
}

inline bool isKeyReleased(const InputState& state, input_key_t key) {
  return std::find(state.keys_released.begin(), state.keys_released.end(), key) != state.keys_released.end();
}

inline bool isKeyRepeated(const InputState& state, input_key_t key) {
  return std::find(state.keys_repeated.begin(), state.keys_repeated.end(), key) != state.keys_repeated.end();
}

inline bool isMouseReleased(const InputState& state, mouse_button_t button) {
  return !isAnyMouseDown(state) &&
         std::find(state.mouse_released.begin(), state.mouse_released.end(), button) != state.mouse_released.end();
}

inline bool isMouseButtonDown(const InputState& state, mouse_button_t button) {
  return isAnyMouseDown(state) && std::find(state.mouse_down.begin(), state.mouse_down.end(), button) != state.mouse_down.end();
}

inline bool isMouseButtonPressed(const InputState& state, mouse_button_t button) {
  return isAnyMouseDown(state) &&
         std::find(state.mouse_pressed.begin(), state.mouse_pressed.end(), button) != state.mouse_pressed.end();
}

inline bool isMouseButtonReleased(const InputState& state, mouse_button_t button) {
  return isFlagSet(input::HAS_KEY_RELEASED, state.flags) &&
         std::find(state.mouse_released.begin(), state.mouse_released.end(), button) != state.mouse_released.end();
}

inline bool isMouseScroll(const InputState& state) {
  return isFlagSet(input::HAS_MOUSE_SCROLL, state.flags);
}

inline bool isMouseScrolled(const InputState& state) {
  return isMouseScroll(state);
}

inline bool isMouseMoved(const InputState& state) {
  return isFlagSet(input::HAS_MOUSE_MOVE, state.flags);
}

inline bool isShiftDown(const InputState& state) {
  return isAnyKeyDown(state) && state.platform_flags & MOD_SHIFT;
}

inline void keyPressed(InputState& state, const input_key_t key, const i32 flags) {
  state.platform_flags = flags;
  state.keys_pressed.insert(key);
  state.keys_down.insert(key);
  setFlag(input::HAS_KEY_PRESSED, true, state.flags);
  setFlag(input::HAS_KEY_DOWN, true, state.flags);
}

inline void keyReleased(InputState& state, const input_key_t key, const i32 flags) {
  state.platform_flags = flags;
  state.keys_released.insert(key);
  state.keys_down.erase(key);
  state.keys_repeated.erase(key);
  setFlag(input::HAS_KEY_RELEASED, state.keys_released.size() > 0, state.flags);
  setFlag(input::HAS_KEY_DOWN, state.keys_down.size() > 0, state.flags);
  setFlag(input::HAS_KEY_REPEATED, state.keys_repeated.size() > 0, state.flags);
}

inline void keyRepeated(InputState& state, const input_key_t key, const i32 flags) {
  state.platform_flags = flags;
  state.keys_repeated.insert(key);
  setFlag(input::HAS_KEY_REPEATED, true, state.flags);
}

inline void mouseButtonPressed(InputState& state, const input_key_t key, const i32 flags) {
  state.platform_flags = flags;
  state.mouse_pressed.insert(key);
  state.mouse_down.insert(key);
  setFlag(input::HAS_MOUSE_PRESSED, true, state.flags);
  setFlag(input::HAS_MOUSE_DOWN, true, state.flags);
}

inline void mouseButtonReleased(InputState& state, const input_key_t key, const i32 flags) {
  state.platform_flags = flags;
  state.mouse_released.insert(key);
  state.mouse_down.erase(key);
  setFlag(input::HAS_MOUSE_RELEASED, state.mouse_released.size() > 0, state.flags);
  setFlag(input::HAS_MOUSE_DOWN, state.mouse_down.size() > 0, state.flags);
}

inline void mouseMoved(InputState& state, const f64 xpos, const f64 ypos) {
  state.cursordx = xpos - state.cursorx;
  state.cursordy = ypos - state.cursory;
  state.cursorx = xpos;
  state.cursory = ypos;
  setFlag(input::HAS_MOUSE_MOVE, true, state.flags);
}

inline void mouseScrolled(InputState& state, const f64 dx, const f64 dy) {
  state.scrolldx = dx;
  state.scrolldy = dy;
  setFlag(input::HAS_MOUSE_SCROLL, true, state.flags);
}

inline void resetMouse(InputState& state) {
  if (isFlagSet(input::HAS_MOUSE_PRESSED, state.flags)) {
    setFlag(input::HAS_MOUSE_PRESSED, false, state.flags);
    state.mouse_pressed.clear();
  }
  if (isFlagSet(input::HAS_MOUSE_RELEASED, state.flags)) {
    setFlag(input::HAS_MOUSE_RELEASED, false, state.flags);
    state.mouse_released.clear();
  }

  if (isFlagSet(input::HAS_MOUSE_MOVE, state.flags)) setFlag(input::HAS_MOUSE_MOVE, false, state.flags);
  if (isFlagSet(input::HAS_MOUSE_SCROLL, state.flags)) setFlag(input::HAS_MOUSE_SCROLL, false, state.flags);

  state.cursordx = 0;
  state.cursordy = 0;
  state.scrolldx = 0;
  state.scrolldy = 0;
}

inline void resetKeyboard(InputState& state) {
  if (isFlagSet(input::HAS_KEY_PRESSED, state.flags)) {
    setFlag(input::HAS_KEY_PRESSED, false, state.flags);
    state.keys_pressed.clear();
  }
  if (isFlagSet(input::HAS_KEY_RELEASED, state.flags)) {
    setFlag(input::HAS_KEY_RELEASED, false, state.flags);
    state.keys_released.clear();
  }
}

inline void reset(InputState& state) {
  resetKeyboard(state);
  resetMouse(state);
}

inline const char* const getKeyStr(input_key_t key) noexcept {
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

inline const char* const getActionStr(input_key_t action) noexcept {
  if (action == ACTION_PRESS) return "press";
  if (action == ACTION_RELEASE) return "release";
  if (action == ACTION_REPEAT) return "repeat";
  return "ACTION UNKNOWN";
}

inline const char* const getModsStr(input_key_t mods) noexcept {
  // TODO implement mods string
  if (mods == 0) return "";
  return "NOT IMPLEMENTED";
}

inline const char* const getMouseButtonStr(mouse_button_t button) noexcept {
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

}  // namespace uinta

#endif  // UINTA_INPUT_HPP
