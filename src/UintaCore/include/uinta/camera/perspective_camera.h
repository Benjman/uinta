#ifndef UINTA_PERSPECTIVE_CAMERA_H
#define UINTA_PERSPECTIVE_CAMERA_H

#include "icamera.h"

#include <uinta/engine_state.h>
#include <uinta/ecs.h>

namespace uinta {

	class PerspectiveCamera : public ICamera, public Component {
		friend class CameraController;

		static constexpr glm::vec3 WORLD_RIGHT = glm::vec3(1.f, 0.f, 0.f);
		static constexpr glm::vec3 WORLD_UP = glm::vec3(0.f, 1.f, 0.f);

		glm::vec3 _target = glm::vec3(0.f);

		float_t _dist = 10.f;
		float_t _pitch = 45.f;
		float_t _yaw = 0.f;

		bool _viewDirty = true;

		void updateDistance(const EngineState &state);
		void updatePitch(const EngineState &state);
		void updatePosition();
		void updateProjectionMatrix(); // why is perspective camera responsible for proj. mat?
		void updateTarget(const EngineState &state);
		void updateViewMatrix();
		void updateYaw(const EngineState &state);

	public:
		[[nodiscard]] const glm::vec3 &getTarget() const { return _target; }
		[[nodiscard]] float_t getDist() const { return _dist; }
		[[nodiscard]] float_t getPitch() const { return _pitch; }
		[[nodiscard]] float_t getYaw() const { return _yaw; }

		void initialize() override;

		void update(const EngineState &state) override;
	};

} // namespace uinta

#endif //UINTA_PERSPECTIVE_CAMERA_H
