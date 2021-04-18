#ifndef UINTA_I_INPUT_MANAGER_H
#define UINTA_I_INPUT_MANAGER_H

#include <uinta/types.h>

namespace uinta {

	using action_code_t = uint8_t;
	using key_code_t = uint16_t;
	using cursor_code_t = uint8_t;

	static const action_code_t ACTION_RELEASE = 0;
	static const action_code_t ACTION_PRESS = 1;
	static const action_code_t ACTION_REPEAT = 2;
	static const action_code_t ACTION_UNKNOWN  = std::numeric_limits<action_code_t>::max();

	static const key_code_t KEY_SPACE = 32;
	static const key_code_t KEY_APOSTROPHE = 39;
	static const key_code_t KEY_COMMA = 44;
	static const key_code_t KEY_MINUS = 45;
	static const key_code_t KEY_PERIOD = 46;
	static const key_code_t KEY_SLASH = 47;
	static const key_code_t KEY_0 = 48;
	static const key_code_t KEY_1 = 49;
	static const key_code_t KEY_2 = 50;
	static const key_code_t KEY_3 = 51;
	static const key_code_t KEY_4 = 52;
	static const key_code_t KEY_5 = 53;
	static const key_code_t KEY_6 = 54;
	static const key_code_t KEY_7 = 55;
	static const key_code_t KEY_8 = 56;
	static const key_code_t KEY_9 = 57;
	static const key_code_t KEY_SEMICOLON = 59;
	static const key_code_t KEY_EQUAL = 61;
	static const key_code_t KEY_A = 65;
	static const key_code_t KEY_B = 66;
	static const key_code_t KEY_C = 67;
	static const key_code_t KEY_D = 68;
	static const key_code_t KEY_E = 69;
	static const key_code_t KEY_F = 70;
	static const key_code_t KEY_G = 71;
	static const key_code_t KEY_H = 72;
	static const key_code_t KEY_I = 73;
	static const key_code_t KEY_J = 74;
	static const key_code_t KEY_K = 75;
	static const key_code_t KEY_L = 76;
	static const key_code_t KEY_M = 77;
	static const key_code_t KEY_N = 78;
	static const key_code_t KEY_O = 79;
	static const key_code_t KEY_P = 80;
	static const key_code_t KEY_Q = 81;
	static const key_code_t KEY_R = 82;
	static const key_code_t KEY_S = 83;
	static const key_code_t KEY_T = 84;
	static const key_code_t KEY_U = 85;
	static const key_code_t KEY_V = 86;
	static const key_code_t KEY_W = 87;
	static const key_code_t KEY_X = 88;
	static const key_code_t KEY_Y = 89;
	static const key_code_t KEY_Z = 90;
	static const key_code_t KEY_LEFT_BRACKET = 91;
	static const key_code_t KEY_BACKSLASH = 92;
	static const key_code_t KEY_RIGHT_BRACKET = 93;
	static const key_code_t KEY_GRAVE_ACCENT = 96;
	static const key_code_t KEY_WORLD_1 = 161;
	static const key_code_t KEY_WORLD_2 = 162;
	static const key_code_t KEY_ESCAPE = 256;
	static const key_code_t KEY_ENTER = 257;
	static const key_code_t KEY_TAB = 258;
	static const key_code_t KEY_BACKSPACE = 259;
	static const key_code_t KEY_INSERT = 260;
	static const key_code_t KEY_DELETE = 261;
	static const key_code_t KEY_RIGHT = 262;
	static const key_code_t KEY_LEFT = 263;
	static const key_code_t KEY_DOWN = 264;
	static const key_code_t KEY_UP = 265;
	static const key_code_t KEY_PAGE_UP = 266;
	static const key_code_t KEY_PAGE_DOWN = 267;
	static const key_code_t KEY_HOME = 268;
	static const key_code_t KEY_END = 269;
	static const key_code_t KEY_CAPS_LOCK = 280;
	static const key_code_t KEY_SCROLL_LOCK = 281;
	static const key_code_t KEY_NUM_LOCK = 282;
	static const key_code_t KEY_PRINT_SCREEN = 283;
	static const key_code_t KEY_PAUSE = 284;
	static const key_code_t KEY_F1 = 290;
	static const key_code_t KEY_F2 = 291;
	static const key_code_t KEY_F3 = 292;
	static const key_code_t KEY_F4 = 293;
	static const key_code_t KEY_F5 = 294;
	static const key_code_t KEY_F6 = 295;
	static const key_code_t KEY_F7 = 296;
	static const key_code_t KEY_F8 = 297;
	static const key_code_t KEY_F9 = 298;
	static const key_code_t KEY_F10 = 299;
	static const key_code_t KEY_F11 = 300;
	static const key_code_t KEY_F12 = 301;
	static const key_code_t KEY_F13 = 302;
	static const key_code_t KEY_F14 = 303;
	static const key_code_t KEY_F15 = 304;
	static const key_code_t KEY_F16 = 305;
	static const key_code_t KEY_F17 = 306;
	static const key_code_t KEY_F18 = 307;
	static const key_code_t KEY_F19 = 308;
	static const key_code_t KEY_F20 = 309;
	static const key_code_t KEY_F21 = 310;
	static const key_code_t KEY_F22 = 311;
	static const key_code_t KEY_F23 = 312;
	static const key_code_t KEY_F24 = 313;
	static const key_code_t KEY_F25 = 314;
	static const key_code_t KEY_KP_0 = 320;
	static const key_code_t KEY_KP_1 = 321;
	static const key_code_t KEY_KP_2 = 322;
	static const key_code_t KEY_KP_3 = 323;
	static const key_code_t KEY_KP_4 = 324;
	static const key_code_t KEY_KP_5 = 325;
	static const key_code_t KEY_KP_6 = 326;
	static const key_code_t KEY_KP_7 = 327;
	static const key_code_t KEY_KP_8 = 328;
	static const key_code_t KEY_KP_9 = 329;
	static const key_code_t KEY_KP_DECIMAL = 330;
	static const key_code_t KEY_KP_DIVIDE = 331;
	static const key_code_t KEY_KP_MULTIPLY = 332;
	static const key_code_t KEY_KP_SUBTRACT = 333;
	static const key_code_t KEY_KP_ADD = 334;
	static const key_code_t KEY_KP_ENTER = 335;
	static const key_code_t KEY_KP_EQUAL = 336;
	static const key_code_t KEY_LEFT_SHIFT = 340;
	static const key_code_t KEY_LEFT_CONTROL = 341;
	static const key_code_t KEY_LEFT_ALT = 342;
	static const key_code_t KEY_LEFT_SUPER = 343;
	static const key_code_t KEY_RIGHT_SHIFT = 344;
	static const key_code_t KEY_RIGHT_CONTROL = 345;
	static const key_code_t KEY_RIGHT_ALT = 346;
	static const key_code_t KEY_RIGHT_SUPER = 347;
	static const key_code_t KEY_MENU = 348;
	static const key_code_t KEY_FIRST = KEY_SPACE;
	static const key_code_t KEY_LAST = KEY_MENU;
	static const key_code_t INVALID_KEY = std::numeric_limits<key_code_t>::max();

	static const cursor_code_t CURSOR_BUTTON_1 = 0;
	static const cursor_code_t CURSOR_BUTTON_2 = 1;
	static const cursor_code_t CURSOR_BUTTON_3 = 2;
	static const cursor_code_t CURSOR_BUTTON_4 = 3;
	static const cursor_code_t CURSOR_BUTTON_5 = 4;
	static const cursor_code_t CURSOR_BUTTON_6 = 5;
	static const cursor_code_t CURSOR_BUTTON_7 = 6;
	static const cursor_code_t CURSOR_BUTTON_8 = 7;
	static const cursor_code_t CURSOR_BUTTON_LAST = CURSOR_BUTTON_8;
	static const cursor_code_t CURSOR_BUTTON_LEFT = CURSOR_BUTTON_1;
	static const cursor_code_t CURSOR_BUTTON_RIGHT = CURSOR_BUTTON_2;
	static const cursor_code_t CURSOR_BUTTON_MIDDLE = CURSOR_BUTTON_3;

	struct InputEvent {
		bool shiftDown = false;
		bool controlDown = false;
		bool altDown = false;
		bool superDown = false;

		action_code_t action = 0;
		key_code_t key = 0;
	};

	class IInputManager {
	protected:
		bool shiftDown = false;
		bool controlDown = false;
		bool altDown = false;
		bool superDown = false;

	public:
		[[nodiscard]] virtual bool isKeyDown(key_code_t key) const = 0;
		[[nodiscard]] virtual int16_t getCursorX() const = 0;
		[[nodiscard]] virtual int16_t getCursorDX() const = 0;
		[[nodiscard]] virtual int16_t getCursorY() const = 0;
		[[nodiscard]] virtual int16_t getCursorDY() const = 0;
		[[nodiscard]] virtual bool isCursorDown(cursor_code_t code) const = 0;

		[[nodiscard]] bool isShiftDown() const {
			return shiftDown;
		}

		[[nodiscard]] bool isControlDown() const {
			return controlDown;
		}

		[[nodiscard]] bool isAltDown() const {
			return altDown;
		}

		[[nodiscard]] bool isSuperDown() const {
			return superDown;
		}

	}; // class IInputManager

} // namespace uinta

#endif //UINTA_I_INPUT_MANAGER_H
