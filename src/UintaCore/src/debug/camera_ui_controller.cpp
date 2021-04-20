#include <uinta/debug/camera_ui_controller.h>
#include <uinta/camera/camera_controller.h>
#include <uinta/model/mesh.h>
#include <string>

using namespace uinta;

CameraUiController::CameraUiController(BufferController *parent, Text &position, Font *font, Text &yaw, Text &zoom,
									   const CameraController *cameraController)
		: TextController(parent, position, font, 6),
		  _cameraController(cameraController),
		  _yaw(parent, yaw, font, cameraController),
		  _zoom(parent, zoom, font, cameraController) {
}

void CameraUiController::initialize() {
	TextController::initialize();
}

void CameraUiController::initializeMeshBuffers(BufferController *buffer) {
	TextController::initializeMeshBuffers(buffer);
	_yaw.initializeMeshBuffers(getParent());
	_zoom.initializeMeshBuffers(getParent());
}

void CameraUiController::update(const EngineState &state) {
	Controller::update(state);
	if (_pitch == _cameraController->getCamera().getPitch()) return;
	_pitch = _cameraController->getCamera().getPitch();
	const std::string &val = "P" + std::to_string(_pitch);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}

YawController::YawController(BufferController *parent, Text &text, Font *font, const CameraController *cameraController)
		: TextController(parent, text, font, 6),
		  _cameraController(cameraController) {}

void YawController::update(const EngineState &state) {
	if (_yaw == _cameraController->getCamera().getYaw()) return;
	_yaw = _cameraController->getCamera().getYaw();
	const std::string &val = "Y" + std::to_string(_yaw);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}

ZoomController::ZoomController(BufferController *parent, Text &text, Font *font,
							   const CameraController *cameraController)
		: TextController(parent, text, font, 6),
		  _cameraController(cameraController) {}

void ZoomController::update(const EngineState &state) {
	if (_zoom == _cameraController->getCamera().getDist()) return;
	_zoom = _cameraController->getCamera().getDist();
	const std::string &val = "Z" + std::to_string(_zoom);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}
