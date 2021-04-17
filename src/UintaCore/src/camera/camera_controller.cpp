#include <uinta/camera/camera_controller.h>
#include <uinta/gl/gl_state.h>
#include <uinta/input/i_input_manager.h>
#include <uinta/math.h> // NOLINT(modernize-deprecated-headers)

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace uinta;

void CameraController::update(const EngineState &state) {
	updatePosition(state);
	updateProjectionMatrix();
	updateViewMatrix();
}

void CameraController::updateProjectionMatrix() {
	_camera._projection = glm::perspective(rad(40.f), gl_state::getViewportAspectRatio(), 0.1f, 100.f);
}

void CameraController::updateViewMatrix() {
	if (_viewDirty) {
		_camera._view = glm::yawPitchRoll(rad(_camera._yaw), rad(_camera._pitch), 0.f);
		_camera._view = glm::translate(_camera._view, _camera._position);
		_viewDirty = false;
	}
}

void CameraController::updateYaw() {
	_camera._yaw = (360.f - _camera._angle);
	fmod(_camera._yaw, 360.f);
}

void CameraController::updatePosition(const EngineState &state) {
	updateZoom(state);
	updatePitch(state);
	updateAngle(state);
	updateYaw();

	if (!_viewDirty) return;

	float_t hDist = _camera._dist * cosf(rad(_camera._pitch));
	float_t vDist = _camera._dist * sinf(rad(_camera._pitch));

	float_t xOff = hDist * sinf(rad(_camera._angle));
	float_t zOff = hDist * cosf(rad(_camera._angle));

	_camera._position.x = _target.x - xOff;
	_camera._position.y = _target.y + vDist;
	_camera._position.z = _target.z - zOff;
}

void CameraController::updateAngle(const EngineState &state) {
	if (state.inputManager->isKeyDown(KEY_A)) {
		if (state.inputManager->isKeyDown(KEY_LEFT)) {
			_camera._angle -= _speed * state.delta;
			_viewDirty = true;
		} else if (state.inputManager->isKeyDown(KEY_RIGHT)) {
			_camera._angle += _speed * state.delta;
			_viewDirty = true;
		}
	}
}

void CameraController::updateZoom(const EngineState &state) {
	if (state.inputManager->isKeyDown(KEY_Z)) {
		if (state.inputManager->isKeyDown(KEY_UP)) {
			_camera._dist -= _speed * state.delta;
			_viewDirty = true;
		} else if (state.inputManager->isKeyDown(KEY_DOWN)) {
			_camera._dist += _speed * state.delta;
			_viewDirty = true;
		}
	}
}

void CameraController::updatePitch(const EngineState &state) {
	if (state.inputManager->isKeyDown(KEY_A)) {
		if (state.inputManager->isKeyDown(KEY_UP)) {
			_camera._pitch -= _speed * state.delta;
			_viewDirty = true;
		} else if (state.inputManager->isKeyDown(KEY_DOWN)) {
			_camera._pitch += _speed * state.delta;
			_viewDirty = true;
		}
	}
}
