#ifndef UINTA_DEBUGUI_H
#define UINTA_DEBUGUI_H

#include <uinta/debug.h>

namespace uinta {

	class DebugUiProto : public Controller, public IRenderable {
		DebugUiController controller = DebugUiController(this);

	public:
		void render() override {
			controller.render();
		}

	}; // class DebugUiProto

} // namespace uinta

#endif // UINTA_DEBUGUI_H