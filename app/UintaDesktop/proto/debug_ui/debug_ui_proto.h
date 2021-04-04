#ifndef UINTA_DEBUGUI_H
#define UINTA_DEBUGUI_H

#include "../proto.h"

#include <uinta/debug.h>

namespace uinta {

	class DebugUiProto : public Proto {
		DebugUiController controller = DebugUiController(this);

	public:
		DebugUiProto() {
			controller.initialize();
		}

		void update(float_t dt) override {
			controller.update(dt);
		}

		void render() override {
			controller.render();
		}

	}; // class DebugUiProto

} // namespace uinta

#endif // UINTA_DEBUGUI_H
