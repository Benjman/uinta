#include <uinta/controller/tick_controller.h>

#include <uinta/model/mesh.h>

#include <string>

using namespace uinta;

void TickController::update(const EngineState &state) {
	Controller::update(state);
	_tick = state.tick;
	setTextValue(std::to_string(_tick).c_str());
	populateMesh();
	uploadMesh(_parent);
}
