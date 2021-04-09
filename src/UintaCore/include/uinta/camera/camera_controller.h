#ifndef UINTA_CAMERA_CONTROLLER_H
#define UINTA_CAMERA_CONTROLLER_H

#include "perspective_camera.h"

#include <uinta/controller/controller.h>

namespace uinta {

	class CameraController : public Controller {
		PerspectiveCamera camera;

		static constexpr float_t SPEED = 2.5f;

	public:
		explicit CameraController(Controller *parent) : Controller(parent) {}

		void update(const EngineState &state) override;

		void initialize() override {
			updateMatrices();
		}

		void updateMatrices();

		[[nodiscard]] glm::mat4 getProjectionMatrix() const { return camera._projection; }
		[[nodiscard]] glm::mat4 getViewMatrix() const { return camera._view; }

	}; // class CameraController

} // namespace uinta

#endif //UINTA_CAMERA_CONTROLLER_H
