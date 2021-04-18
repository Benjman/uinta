#include <uinta/input/input_manager.h>

#include <array>
#include <cstring> // for memcpy

using namespace uinta;

InputManager::InputManager() {
	reset();
}

void InputManager::registerEvent(const InputEvent &event) {
	if (event.action == ACTION_PRESS) return handlePressEvent(event);
	if (event.action == ACTION_RELEASE) return handleReleaseEvent(event);
	if (event.action == ACTION_REPEAT) return handleRepeatEvent(event);
}

void InputManager::reset() {
	std::fill(&downKeys[0], &downKeys[KEY_LAST - KEY_FIRST], false);
}

void InputManager::setCursor(int16_t x, int16_t y, int16_t xScroll, int16_t yScroll, bool *cursorButtonsState) {
	cursorDX = x - cursorX;
	cursorDY = y - cursorY;

	cursorX = x;
	cursorY = y;

	cursorXScroll = xScroll;
	cursorYScroll = yScroll;

	memcpy(cursorButtonsDown, cursorButtonsState, sizeof(cursorButtonsDown));
}
