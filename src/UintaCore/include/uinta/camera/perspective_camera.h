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
		float_t _pitch = 0.f;
		float_t _yaw = 0.f;

	}; // class PerspectiveCamera

} // namespace uinta

#endif //UINTA_PERSPECTIVE_CAMERA_H
