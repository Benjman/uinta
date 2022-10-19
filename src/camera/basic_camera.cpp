#include <glm/geometric.hpp>
#include <uinta/camera/basic_camera.hpp>
#include <uinta/utils/direction.hpp>

uinta::BasicCamera::BasicCamera() {
  position = WORLD_UP;
  pitch = 0;
  yaw = -90;  // this will point the camera to origin if WORLD_UP = {0, 1, 0}]
}

uinta::BasicCamera::BasicCamera(glm::vec3 position, float pitch, float yaw) : position(position), pitch(pitch), yaw(yaw) {
}

uinta::BasicCamera& uinta::BasicCamera::operator=(const BasicCamera& other) {
  position = glm::vec3(other.position);
  pitch = other.pitch;
  yaw = other.yaw;
  return *this;
}
