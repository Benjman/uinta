#ifndef UINTA_DEBUG_CONTROLLER_H
#define UINTA_DEBUG_CONTROLLER_H

#include "debug_ui_controller.h"
#include "debug_world_controller.h"

#include <uinta/camera/icamera.h>

namespace uinta {

	class Font;

	class DebugController : public Controller {
		DebugUiController _ui = DebugUiController(this);
		DebugWorldController _world;

	public:
		DebugController(Controller *parent, const CameraController *camera);

		void render();

	};

} // namespace uinta

#endif //UINTA_DEBUG_CONTROLLER_H
