#ifndef UINTA_ENGINE_STATE_H
#define UINTA_ENGINE_STATE_H

#include "types.h"

namespace uinta {

	class IInputManager;

	struct EngineState {
		size_t tick = 0;
		float_t runtime = 0.f;
		float_t delta = 0.f;

		const IInputManager *inputManager;

		explicit EngineState(const IInputManager *manager) : inputManager(manager) {}

	}; // struct EngineState

} // namespace uinta

#endif //UINTA_ENGINE_STATE_H
