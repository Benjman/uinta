#ifndef UINTA_INPUT_H
#define UINTA_INPUT_H

#include "i_input_manager.h"

namespace uinta {

	struct InputManager : public IInputManager {
		bool downKeys[KEY_LAST - KEY_FIRST]{};

		InputManager();

		void registerEvent(const InputEvent &event);

		void reset();

		[[nodiscard]] bool isKeyDown(key_code_t key) const override {
			return downKeys[key - KEY_FIRST];
		}

	private:
		void handlePressEvent(const InputEvent &event) {
			downKeys[event.key - KEY_FIRST] = true;
		}

		void handleReleaseEvent(const InputEvent &event) {
			downKeys[event.key - KEY_FIRST] = false;
		}

		void handleRepeatEvent(const InputEvent &event) {}

	}; // struct InputManager

} // namespace uinta

#endif //UINTA_INPUT_H
