#include <uinta/controller/fps_controller.h>
#include <uinta/model/mesh.h>
#include <uinta/text/text.h>

#include <string>

using namespace uinta;

FpsController::FpsController(BufferController *parent, Text &text, Font *font)
		: TextController(parent, text, (Font *) font, 3),
		  _parent(parent) {
}

void FpsController::update(const EngineState &state) {
	Controller::update(state);
	_timeToNextUpdate -= state.delta;
	if (_timeToNextUpdate <= 0) {
		if (_curFps != _frameCount) {
			setTextValue(std::to_string((size_t) std::ceil((float_t) _frameCount * (1 / INTERVAL))).c_str());
			populateMesh();
			uploadMesh(_parent);
		}

		_frameCount = 0;
		_timeToNextUpdate = INTERVAL;
	}
}
