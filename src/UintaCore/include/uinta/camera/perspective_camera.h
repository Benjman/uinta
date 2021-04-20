#ifndef UINTA_PERSPECTIVE_CAMERA_H
#define UINTA_PERSPECTIVE_CAMERA_H

#include "icamera.h"

#include <uinta/engine_state.h>

namespace uinta {

	class PerspectiveCamera : public ICamera {
		friend class CameraController;

		glm::vec3 _target = glm::vec3(0.f);

		float_t _angle = 0.f;
		float_t _dist = 10.f;
		float_t _pitch = 45.f;
		float_t _yaw = 0.f;

	public:
		[[nodiscard]] const glm::vec3 &getTarget() const { return _target; }
		[[nodiscard]] float_t getAngle() const { return _angle; }
		[[nodiscard]] float_t getDist() const { return _dist; }
		[[nodiscard]] float_t getPitch() const { return _pitch; }
		[[nodiscard]] float_t getYaw() const { return _yaw; }

	};

} // namespace uinta

#endif //UINTA_PERSPECTIVE_CAMERA_H
