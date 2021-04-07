#ifndef UINTA_INPUT_H
#define UINTA_INPUT_H

#include "i_input_manager.h"

namespace uinta {

	struct InputManager : public IInputManager {
		static const size_t MAX_KEYS_DOWN = 4;

		key_code_t downKeys[MAX_KEYS_DOWN]{};

		InputManager();

		void registerEvent(const InputEvent &event);

		void reset();

		[[nodiscard]] bool isKeyDown(key_code_t key) const override {
			for (uint16_t downKey : downKeys) {
				if (downKey == key) return true;
				if (downKey == INVALID_KEY) return false;
			}
			return false;
		}

	private:
		void handlePressEvent(const InputEvent &event);

		void handleReleaseEvent(const InputEvent &event);

		void handleRepeatEvent(const InputEvent &event);

	}; // struct InputManager

} // namespace uinta

#endif //UINTA_INPUT_H
