#ifndef UINTA_CAMERA_CONTROLLER_H
#define UINTA_CAMERA_CONTROLLER_H

#include "perspective_camera.h"

#include <uinta/controller/controller.h>

namespace uinta {

	class CameraController : public Controller {
		PerspectiveCamera _camera;

		float_t _zoomSpeed = 12.f;
		float_t _yawSpeed = 3.f;
		bool _viewDirty = true;

		void updateYaw();

		void updatePitch(const EngineState &state);

		void updatePosition(const EngineState &state);

		void updateAngle(const EngineState &state);

		void updateZoom(const EngineState &state);

		void updateTarget(const EngineState &state);

	public:
		explicit CameraController(Controller *parent) : Controller(parent) {}

		void update(const EngineState &state) override;

		void initialize() override {
			updateProjectionMatrix();
			updateViewMatrix();
			_viewDirty = true; // forces update of position in first update
		}

		void updateProjectionMatrix();

		void updateViewMatrix();

		[[nodiscard]] const PerspectiveCamera &getCamera() const { return _camera; }
		[[nodiscard]] glm::mat4 getProjectionMatrix() const { return _camera._projection; }
		[[nodiscard]] glm::mat4 getViewMatrix() const { return _camera._view; }

	}; // class CameraController

} // namespace uinta

#endif //UINTA_CAMERA_CONTROLLER_H
