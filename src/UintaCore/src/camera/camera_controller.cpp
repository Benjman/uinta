#include <uinta/camera/camera_controller.h>
#include <uinta/gl/gl_state.h>
#include <uinta/input/i_input_manager.h>
#include <uinta/math.h> // NOLINT(modernize-deprecated-headers)

#include <glm/gtc/matrix_transform.hpp>

using namespace uinta;

void CameraController::update(const EngineState &state) {
	Controller::update(state);
	calculateAngle(state);
	calculatePitch(state);
	calculateZoom(state);
	calculateCameraPosition();
	calculateTarget(state);
	updateMatrices();
}

void CameraController::calculateTarget(const EngineState &state) {
	float speed = SPEED * state.delta;
	if (state.inputManager->isKeyDown(KEY_UP)) {
		camera._target += speed * ICamera::FORWARD;
	}
	if (state.inputManager->isKeyDown(KEY_DOWN)) {
		camera._target -= speed * ICamera::FORWARD;
	}
	if (state.inputManager->isKeyDown(KEY_RIGHT)) {
		camera._target += speed * glm::normalize(glm::cross(ICamera::FORWARD, ICamera::UP));
	}
	if (state.inputManager->isKeyDown(KEY_LEFT)) {
		camera._target -= speed * glm::normalize(glm::cross(ICamera::FORWARD, ICamera::UP));
	}
}

void CameraController::updateMatrices() {
	camera._view = glm::translate(glm::mat4(1.f), camera._position);
	camera._projection = glm::perspective(glm::radians(40.f), gl_state::getViewportAspectRatio(), 0.1f, 100.f);
}

void CameraController::calculateZoom(const EngineState &state) {
	if (state.inputManager->isKeyDown(KEY_EQUAL)) {
		camera._dist += state.delta * 2.5f;
	}
	if (state.inputManager->isKeyDown(KEY_MINUS)) {
		camera._dist -= state.delta * 2.5f;
	}
}

void CameraController::calculatePitch(const EngineState &state) {
	if (state.inputManager->isKeyDown(KEY_RIGHT_BRACKET)) {
		camera._pitch += state.delta * 2.5f;
	}
	if (state.inputManager->isKeyDown(KEY_LEFT_BRACKET)) {
		camera._pitch -= state.delta * 2.5f;
	}
}

void CameraController::calculateAngle(const EngineState &state) {
	if (state.inputManager->isKeyDown(KEY_APOSTROPHE)) {
		camera._angle += state.delta * 2.5f;
	}
	if (state.inputManager->isKeyDown(KEY_SEMICOLON)) {
		camera._angle -= state.delta * 2.5f;
	}
}

float_t CameraController::calculateDistanceHorizontal() const {
	return camera._dist * (float_t) cos(rad(camera._pitch));
}

float_t CameraController::calculateDistanceVertical() const {
	return camera._dist * (float_t) sin(rad(camera._pitch));
}

void CameraController::calculateCameraPosition() {
	float_t hDist = calculateDistanceHorizontal();
	float_t vDist = calculateDistanceVertical();
	float_t xOffset = hDist * (float_t) sin(rad(camera._angle));
	float_t zOffset = hDist * (float_t) cos(rad(camera._angle));

	camera._position.x = camera._target.x - xOffset;
	camera._position.y = camera._target.y + vDist;
	camera._position.z = camera._target.z - zOffset;

	// TODO yaw
}
