#include <uinta/input/input_manager.h>

#include <iostream>
#include <array>

using namespace uinta;

InputManager::InputManager() {
	std::fill(std::begin(downKeys), std::end(downKeys), INVALID_KEY);
}

void InputManager::registerEvent(const InputEvent &event) {
	if (event.action == ACTION_PRESS) return handlePressEvent(event);
	if (event.action == ACTION_RELEASE) return handleReleaseEvent(event);
	if (event.action == ACTION_REPEAT) return handleRepeatEvent(event);
}

void InputManager::reset() {
}

void InputManager::handlePressEvent(const InputEvent &event) {
	for (uint16_t &downKey : downKeys) {
		if (downKey == INVALID_KEY) {
			downKey = event.key;
			return;
		}
	}
	std::cerr << "Maximum inputs exceeded! Ignoring inputs.\n";
}

void InputManager::handleReleaseEvent(const InputEvent &event) {
	for (size_t i = 0; i < MAX_KEYS_DOWN; i++) {
		if (downKeys[i] == event.key) {
			for (size_t ii = i + 1; ii < MAX_KEYS_DOWN; i++, ii++) {
				downKeys[i] = downKeys[ii];
				if (downKeys[i] == INVALID_KEY) {
					return;
				}
			}
			downKeys[MAX_KEYS_DOWN - 1] = INVALID_KEY;
			return;
		}
	}
}

void InputManager::handleRepeatEvent(const InputEvent &event) {
}
