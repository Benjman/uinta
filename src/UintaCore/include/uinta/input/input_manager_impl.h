#ifndef UINTA_INPUT_H
#define UINTA_INPUT_H

#include "../input_manager.h"

namespace uinta {

	class InputManagerImpl : public InputManager {
		int16_t cursorX = 0;
		int16_t cursorDX = 0;
		int16_t cursorY = 0;
		int16_t cursorDY = 0;
		float_t cursorXScroll = 0;
		float_t cursorYScroll = 0;
		bool cursorButtonsDown[8]{false};

		bool downKeys[KEY_LAST - KEY_FIRST]{};

		void handlePressEvent(const InputEvent &event) {
			downKeys[event.key - KEY_FIRST] = true;
		}

		void handleReleaseEvent(const InputEvent &event) {
			downKeys[event.key - KEY_FIRST] = false;
		}

		void handleRepeatEvent(const InputEvent &event) {}

	public:
		InputManagerImpl();

		void registerEvent(const InputEvent &event);

		void reset();

		[[nodiscard]] bool isKeyDown(key_code_t key) const override {
			return downKeys[key - KEY_FIRST];
		}

		void setCursor(int16_t x, int16_t y, int16_t xScroll, int16_t yScroll, bool *cursorButtonsState);

		[[nodiscard]] int16_t getCursorX() const override {
			return cursorX;
		}

		[[nodiscard]] int16_t getCursorDX() const override {
			return cursorDX;
		}

		[[nodiscard]] float_t getCursorXScroll() const override {
			return cursorXScroll;
		}

		[[nodiscard]] int16_t getCursorY() const override {
			return cursorY;
		}

		[[nodiscard]] int16_t getCursorDY() const override {
			return cursorDY;
		}

		[[nodiscard]] float_t getCursorYScroll() const override {
			return cursorYScroll;
		}

		[[nodiscard]] bool isCursorDown(cursor_code_t code) const override {
			return cursorButtonsDown[code];
		}

	}; // struct InputManager

} // namespace uinta

#endif //UINTA_INPUT_H
