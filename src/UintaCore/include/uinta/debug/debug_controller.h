#ifndef UINTA_DEBUG_CONTROLLER_H
#define UINTA_DEBUG_CONTROLLER_H

#include "debug_ui.h"

namespace uinta {

	class DebugController {
		DebugUi _ui;

	public:
		DebugController();

		DebugUi &getUi() { return _ui; }

		void render() {
			_ui.render();
		}

	}; // class DebugController 

} // namespace uinta

#endif //UINTA_DEBUG_CONTROLLER_H