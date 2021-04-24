#ifndef UINTA_PERSPECTIVE_CAMERA_H
#define UINTA_PERSPECTIVE_CAMERA_H

#include "icamera.h"

#include <uinta/data/smooth_values.h>
#include <uinta/ecs.h>
#include <uinta/engine_state.h>

namespace uinta {

	class PerspectiveCamera : public ICamera, public Component {
		friend class CameraController;

		static constexpr glm::vec3 WORLD_RIGHT = glm::vec3(1.f, 0.f, 0.f);
		static constexpr glm::vec3 WORLD_UP = glm::vec3(0.f, 1.f, 0.f);

		static constexpr float_t DIST_SENSITIVITY = 80.f;
		static constexpr float_t PITCH_SENSITIVITY = 10.f;
		static constexpr float_t POSITION_SENSITIVITY = 0.01f;
		static constexpr float_t YAW_SENSITIVITY = 10.f;

		SmoothVector3 _target = SmoothVector3(glm::vec3(0.f, 0.f, 0.f), 10.f);
		SmoothFloat _dist = SmoothFloat(15.f, 15.f);
		SmoothFloat _pitch = SmoothFloat(35.f, 15.f);
		SmoothFloat _yaw = SmoothFloat(30.f, 15.f);

		void updateDistance(const EngineState &state);
		void updatePitch(const EngineState &state);
		void updatePosition();
		void updateProjectionMatrix(); // why is perspective camera responsible for proj. mat?
		void updateTarget(const EngineState &state);
		void updateViewMatrix();
		void updateYaw(const EngineState &state);

	public:
		[[nodiscard]] glm::vec3 getTarget() const { return _target; }
		[[nodiscard]] float_t getDist() const { return _dist; }
		[[nodiscard]] float_t getPitch() const { return _pitch; }
		[[nodiscard]] float_t getYaw() const { return _yaw; }

		void initialize() override;

		void update(const EngineState &state) override;
	};

} // namespace uinta

#endif //UINTA_PERSPECTIVE_CAMERA_H
