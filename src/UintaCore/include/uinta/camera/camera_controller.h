#ifndef UINTA_CAMERA_CONTROLLER_H
#define UINTA_CAMERA_CONTROLLER_H

#include "perspective_camera.h"

#include <uinta/controller/controller.h>

namespace uinta {

	class CameraController : public Controller {
		PerspectiveCamera _camera;
		glm::vec3 _target = glm::vec3(0);

		float_t _speed = 15.f;

		bool _viewDirty = true;

		void updateYaw();

		void updatePitch(const EngineState &state);

		void updatePosition(const EngineState &state);

		void updateAngle(const EngineState &state);

		void updateZoom(const EngineState &state);

	public:
		explicit CameraController(Controller *parent) : Controller(parent) {}

		void update(const EngineState &state) override;

		void initialize() override {
			updateProjectionMatrix();
			updateViewMatrix();
			_viewDirty = true; // update position in first update
		}

		void updateProjectionMatrix();

		void updateViewMatrix();

		[[nodiscard]] glm::mat4 getProjectionMatrix() const { return _camera._projection; }
		[[nodiscard]] glm::mat4 getViewMatrix() const { return _camera._view; }

	}; // class CameraController

} // namespace uinta

#endif //UINTA_CAMERA_CONTROLLER_H
