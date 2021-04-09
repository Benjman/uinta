#include <uinta/camera/camera_controller.h>
#include <uinta/gl/gl_state.h>
#include <uinta/input/i_input_manager.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace uinta;

void CameraController::update(const EngineState &state) {
	Controller::update(state);
	float speed = SPEED * state.delta;
	bool positionChanged = false;
	if (state.inputManager->isKeyDown(KEY_UP)) {
		camera._position += speed * ICamera::FORWARD;
		positionChanged = true;
	}
	if (state.inputManager->isKeyDown(KEY_DOWN)) {
		camera._position -= speed * ICamera::FORWARD;
		positionChanged = true;
	}
	if (state.inputManager->isKeyDown(KEY_RIGHT)) {
		camera._position += speed * glm::normalize(glm::cross(ICamera::FORWARD, ICamera::UP));
		positionChanged = true;
	}
	if (state.inputManager->isKeyDown(KEY_LEFT)) {
		camera._position -= speed * glm::normalize(glm::cross(ICamera::FORWARD, ICamera::UP));
		positionChanged = true;
	}
	if (positionChanged) {
		updateMatrices();
	}
}

void CameraController::updateMatrices() {
	camera._view = glm::translate(glm::mat4(1.f), camera._position);
	camera._projection = glm::perspective(glm::radians(40.f), gl_state::getViewportAspectRatio(), 0.1f, 100.f);
}
