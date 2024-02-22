#ifndef SRC_PLATFORM_INCLUDE_UINTA_INPUT_FWD_H_
#define SRC_PLATFORM_INCLUDE_UINTA_INPUT_FWD_H_

#include "uinta/types.h"

namespace uinta {

using InputKey = i32;
using InputMod = i32;
using MouseButton = i32;

namespace _ {
constexpr InputKey InputUnknown = -1;
constexpr size_t InputStorageSize = 8;
}  // namespace _

struct KeyboardEvent {
  InputKey key;
  i32 scancode;
  i32 action;
  InputMod mods;
};

struct MouseEvent {
  f64 x;
  f32 y;
  MouseButton button;
  i32 action;
  InputMod mods;
};

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

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_INPUT_FWD_H_
