#ifndef UINTA_CAMERA_CONTROLLER_H
#define UINTA_CAMERA_CONTROLLER_H

#include "perspective_camera.h"

#include <uinta/controller/controller.h>

namespace uinta {

	class CameraController : public Controller {
		PerspectiveCamera camera;

		static constexpr float_t SPEED = 2.5f;

		void calculateZoom(const EngineState &state);

		void calculatePitch(const EngineState &state);

		void calculateAngle(const EngineState &state);

		[[nodiscard]] float_t calculateDistanceHorizontal() const;

		[[nodiscard]] float_t calculateDistanceVertical() const;

		void calculateCameraPosition();

		void calculateYaw();


	public:
		explicit CameraController(Controller *parent) : Controller(parent) {}

		void update(const EngineState &state) override;

		void initialize() override {
			updateMatrices();
		}

		void updateMatrices();

		[[nodiscard]] glm::mat4 getProjectionMatrix() const { return camera._projection; }
		[[nodiscard]] glm::mat4 getViewMatrix() const { return camera._view; }

		void calculateTarget(const EngineState &state);
	}; // class CameraController

} // namespace uinta

#endif //UINTA_CAMERA_CONTROLLER_H