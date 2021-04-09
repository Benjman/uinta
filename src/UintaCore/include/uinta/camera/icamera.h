#ifndef UINTA_CAMERA_H
#define UINTA_CAMERA_H

#include <glm/glm.hpp>

namespace uinta {

	class EngineState;

	class ICamera {
		friend class CameraController;

		glm::vec3 _position = glm::vec3(0.f, 0.f, -3.f);
		glm::mat4 _projection = glm::mat4(1);
		glm::mat4 _view = glm::mat4(1);

	public:
		static constexpr glm::vec3 FORWARD = glm::vec3(0.f, 0.f, 1.f);
		static constexpr glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f);
	};

} // namespace uinta

#endif //UINTA_CAMERA_H
