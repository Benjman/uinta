#ifndef UINTA_CAMERA_H
#define UINTA_CAMERA_H

#include <glm/glm.hpp>

namespace uinta {

	class EngineState;

	class ICamera {
	protected:
		glm::vec3 _position = glm::vec3(0.f, 0.f, -3.f);
		glm::mat4 _projection = glm::mat4(1);
		glm::mat4 _view = glm::mat4(1);

	public:
		[[nodiscard]] virtual glm::mat4 getViewMatrix() const { return _view; }
		[[nodiscard]] virtual glm::mat4 getProjectionMatrix() const { return _projection; }

	};

} // namespace uinta

#endif //UINTA_CAMERA_H
