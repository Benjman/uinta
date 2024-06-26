#include "uinta/input.h"

#include "absl/log/log.h"

namespace uinta {

const std::string Input::GetKeyStr(InputKey key) noexcept {
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
  LOG(FATAL) << "Unknown key.";
}

const std::string Input::GetActionStr(InputKey action) noexcept {
  if (action == ACTION_PRESS) return "press";
  if (action == ACTION_RELEASE) return "release";
  if (action == ACTION_REPEAT) return "repeat";
  LOG(FATAL) << "Unknown action.";
}

const std::string Input::GetModsStr(InputKey) noexcept {
  // TODO:
  LOG(FATAL) << "Unimplemented.";
}

const std::string Input::GetMouseButtonStr(MouseButton button) noexcept {
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
  LOG(FATAL) << "Unknown button.";
}

}  // namespace uinta
