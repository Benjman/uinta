#ifndef UINTA_CAMERA_FWD_HPP
#define UINTA_CAMERA_FWD_HPP

#include <uinta/fwd.hpp>
#include <uinta/math/fwd.hpp>

namespace uinta {

struct BasicCamera;
struct CameraConfig;
struct StaticCamera;
struct TargetCamera;

void update(TargetCamera&, const RunnerState&, const InputState&);
glm::mat4 getViewMatrix(const TargetCamera&);
glm::mat4 getPerspectiveMatrix(const TargetCamera&, const Display&);
void getPerspectiveMatrix(glm::mat4* const ref, const TargetCamera&, const Display&);
glm::mat4 getOrthographicMatrix(const TargetCamera&);
void getOrthographicMatrix(glm::mat4* const ref, const TargetCamera&);
inline float calculateTranslationFactor(const TargetCamera&);

}  // namespace uinta

#endif  // UINTA_CAMERA_FWD_HPP
