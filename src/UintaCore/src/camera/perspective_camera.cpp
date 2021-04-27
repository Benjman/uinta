#include <uinta/camera/perspective_camera.h>
#include <uinta/gl/gl_state.h>
#include <uinta/input/input_manager_impl.h>
#include <uinta/math.h> // NOLINT(modernize-deprecated-headers)

#include <glm/gtc/matrix_transform.hpp>

using namespace uinta;

void PerspectiveCamera::update(const EngineState &state) {
	updatePitch(state);
	updateYaw(state);

	updateDistance(state);
	updateTarget(state);

	updatePosition();
	updateViewMatrix();
}

void PerspectiveCamera::initialize() {
	updatePosition();
	updateViewMatrix();
	updateProjectionMatrix();
}

void PerspectiveCamera::updatePitch(const EngineState &state) {
	const InputManager *inputManager = state.inputManager;
	if (inputManager->isCursorDown(CURSOR_BUTTON_RIGHT)) {
		float_t pitchDelta = (float_t) inputManager->getCursorDY() * state.delta * PITCH_SENSITIVITY;
		_pitch += pitchDelta;
	}
	_pitch.update(state.delta);
}

void PerspectiveCamera::updatePosition() {
	// yaw, pitch, dist in relation to target
	float_t xDist = _dist * cosf(rad(_pitch));
	float_t yDist = _dist * sinf(rad(_pitch));

	_position.x = _target.current.x + xDist * sinf(rad(-_yaw));
	_position.y = _target.current.y + yDist;
	_position.z = _target.current.z + xDist * cosf(rad(-_yaw));
}

void PerspectiveCamera::updateTarget(const EngineState &state) {
	const InputManager *inputManager = state.inputManager;
	if (inputManager->isCursorDown(CURSOR_BUTTON_LEFT)) {
		float_t yawCos = cosf(rad(_yaw));
		float_t yawSin = sinf(rad(_yaw));

		float_t h = inputManager->getCursorDX();
		float_t v = inputManager->getCursorDY();

		_target.target.x -= (yawCos * h - yawSin * v) * POSITION_SENSITIVITY;
		_target.target.z -= (yawCos * v + yawSin * h) * POSITION_SENSITIVITY;
	}
	_target.update(state.delta);
}

void PerspectiveCamera::updateYaw(const EngineState &state) {
	if (!state.inputManager->isKeyDown(KEY_SPACE)) {
		_yaw += 15.f * state.delta;
	}
	const InputManager *inputManager = state.inputManager;
	if (inputManager->isCursorDown(CURSOR_BUTTON_RIGHT)) {
		float_t yawDelta = (float_t) inputManager->getCursorDX() * state.delta * YAW_SENSITIVITY;
		_yaw += yawDelta;
	}
	_yaw.update(state.delta);
}

void PerspectiveCamera::updateDistance(const EngineState &state) {
	const InputManager *inputManager = state.inputManager;
	if (inputManager->getCursorYScroll() != 0) {
		float_t distanceDelta = inputManager->getCursorYScroll() * state.delta * DIST_SENSITIVITY;
		_dist -= distanceDelta;
	}
	_dist.update(state.delta);
}

void PerspectiveCamera::updateViewMatrix() {
	_view = glm::rotate(glm::mat4(1.f), rad(_pitch), WORLD_RIGHT);
	_view = glm::rotate(_view, rad(_yaw), WORLD_UP);
	_view = glm::translate(_view, -_position);
}

void PerspectiveCamera::updateProjectionMatrix() {
	_projection = glm::perspective(rad(40.f), gl_state::getViewportAspectRatio(), 0.1f, 100.f);
}
