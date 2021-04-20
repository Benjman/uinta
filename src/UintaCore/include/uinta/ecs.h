#ifndef UINTA_COMPONENT_H
#define UINTA_COMPONENT_H

#include <uinta/engine_state.h>

namespace uinta {

	struct Component {

		virtual void initialize() {}

		virtual void update(const EngineState &state) {}

	}; // struct component

} // namespace uinta

#endif //UINTA_COMPONENT_H
