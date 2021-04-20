#include <uinta/debug/camera_ui_controller.h>
#include <uinta/camera/perspective_camera.h>
#include <uinta/model/mesh.h>
#include <string>

using namespace uinta;

CameraUiController::CameraUiController(BufferController *parent, Text &position, Font *font, Text &yaw, Text &zoom,
									   const PerspectiveCamera *camera)
		: TextController(parent, position, font, 6),
		  _camera(camera),
		  _yaw(parent, yaw, font, camera),
		  _zoom(parent, zoom, font, camera) {
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
	if (_pitch == _camera->getPitch()) return;
	_pitch = _camera->getPitch();
	const std::string &val = "P" + std::to_string(_pitch);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}

YawController::YawController(BufferController *parent, Text &text, Font *font, const PerspectiveCamera *camera)
		: TextController(parent, text, font, 6),
		  _camera(camera) {}

void YawController::update(const EngineState &state) {
	if (_yaw == _camera->getYaw()) return;
	_yaw = _camera->getYaw();
	const std::string &val = "Y" + std::to_string(_yaw);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}

ZoomController::ZoomController(BufferController *parent, Text &text, Font *font,
							   const PerspectiveCamera *camera)
		: TextController(parent, text, font, 6),
		  _camera(camera) {}

void ZoomController::update(const EngineState &state) {
	if (_zoom == _camera->getDist()) return;
	_zoom = _camera->getDist();
	const std::string &val = "Z" + std::to_string(_zoom);
	setTextValue(val.c_str());
	populateMesh();
	uploadMesh();
}
