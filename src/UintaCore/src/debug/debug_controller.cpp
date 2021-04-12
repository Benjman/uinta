#include <uinta/debug/debug_controller.h>
#include <uinta/shader.h>
#include <uinta/text.h>

using namespace uinta;

DebugController::DebugController(Controller *parent)
		: Controller(parent) {
}

void DebugController::render() {
	_ui.render();
}
