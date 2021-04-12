#ifndef UINTA_DEBUG_WORLD_CONTROLLER_H
#define UINTA_DEBUG_WORLD_CONTROLLER_H

#include <uinta/controller/buffer_controller.h>
#include <uinta/render/i_render_controller.h>

namespace uinta {

	class DebugController;

	class DebugWorldController : public BufferController, public IRenderController {
	public:
		explicit DebugWorldController(DebugController *parent);

	}; // class DebugWorldController

} // namespace uinta

#endif //UINTA_DEBUG_WORLD_CONTROLLER_H
