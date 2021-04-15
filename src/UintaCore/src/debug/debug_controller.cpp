#include <uinta/debug/debug_controller.h>
#include <uinta/text.h>

using namespace uinta;

DebugController::DebugController(Controller *parent, const CameraController *camera)
		: Controller(parent),
		_world(this, camera) {
}

void DebugController::render() {
	_ui.render();
	_world.render();
}
