#ifndef UINTA_CAMERA_FWD_HPP
#define UINTA_CAMERA_FWD_HPP

#include <uinta/types.h>

#include <glm/mat4x4.hpp>

namespace uinta {

struct CameraConfig;
struct InputState;
struct RunnerState;
struct TargetCamera;

void updateCamera(TargetCamera&, const RunnerState&, const InputState&);
glm::mat4 getViewMatrix(const TargetCamera&);
glm::mat4 getPerspectiveMatrix(const TargetCamera&);
void getPerspectiveMatrix(glm::mat4* const ref, const TargetCamera&);
glm::mat4 getOrthographicMatrix(const TargetCamera&);
void getOrthographicMatrix(glm::mat4* const ref, const TargetCamera&);

}  // namespace uinta

#endif  // UINTA_CAMERA_FWD_HPP
