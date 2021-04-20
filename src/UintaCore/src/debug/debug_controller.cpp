#include <uinta/debug/debug_controller.h>
#include <uinta/text.h>

using namespace uinta;

DebugController::DebugController(Controller *parent, const CameraController *camera)
		: Controller(parent),
		_world(this, camera),
		_ui (this, camera) {
}

void DebugController::render() {
	_world.render();
	_ui.render();
}
