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

void InputManager::setCursor(int16_t x, int16_t y, bool *cursorButtonsState) {
	cursorDX = x - cursorX;
	cursorDY = y - cursorY;

	cursorX = x;
	cursorY = y;

	memcpy(cursorButtonsDown, cursorButtonsState, sizeof(cursorButtonsDown));
}
