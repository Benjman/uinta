#include <uinta/debug/camera_ui_controller.h>
#include <uinta/camera/perspective_camera.h>
#include <string>

using namespace uinta;

void CameraUiController::update(const EngineState &state) {
	Controller::update(state);
	if (_pitch == _camera->getPitch()) return;
	_pitch = _camera->getPitch();
	const std::string &val = "P" + std::to_string(_pitch);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}

void YawController::update(const EngineState &state) {
	if (_yaw == _camera->getYaw()) return;
	_yaw = _camera->getYaw();
	const std::string &val = "Y" + std::to_string(_yaw);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}

void ZoomController::update(const EngineState &state) {
	if (_zoom == _camera->getDist()) return;
	_zoom = _camera->getDist();
	const std::string &val = "Z" + std::to_string(_zoom);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}
