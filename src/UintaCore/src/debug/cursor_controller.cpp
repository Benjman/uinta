#include <uinta/debug/cursor_controller.h>
#include <uinta/input/i_input_manager.h>

#include <string>

using namespace uinta;

CursorController::CursorController(BufferController *parent, Text &text, Font *font)
		: TextController(parent, text, (Font *) font, 12) {
}

void CursorController::update(const EngineState &state) {
	Controller::update(state);
	if (state.inputManager->getCursorX() == _x && state.inputManager->getCursorY() == _y
		&& state.inputManager->getCursorDX() == _dx && state.inputManager->getCursorDY() == _dy)
		return;
	_x = state.inputManager->getCursorX();
	_y = state.inputManager->getCursorY();
	std::string result = std::to_string(_x) + "x" + std::to_string(_y);
	setTextValue(result.c_str());
	populateMesh();
	uploadMesh();

	_dx = state.inputManager->getCursorDX();
	_dy = state.inputManager->getCursorDY();
}
