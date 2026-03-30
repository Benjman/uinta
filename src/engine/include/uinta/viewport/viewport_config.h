#ifndef SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_CONFIG_H_
#define SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_CONFIG_H_

#include <string>

#include "uinta/types.h"

namespace uinta {

enum class ProjectionType : u8 {
  Perspective,
  Orthographic,
  Unknown,
};

inline std::string toString(ProjectionType projection) noexcept {
  switch (projection) {
    case ProjectionType::Orthographic:
      return "Orthographic";
    case ProjectionType::Perspective:
      return "Perspective";
    default:
      return "Unknown";
  }
}

inline ProjectionType toProjectionType(const std::string& projection) noexcept {
  if ("Perspective" == projection) {
    return ProjectionType::Perspective;
  }
  if ("Orthographic" == projection) {
    return ProjectionType::Orthographic;
  }
  return ProjectionType::Unknown;
}

struct ViewportConfig {
  ProjectionType projection = ProjectionType::Unknown;
  f32 fov = 45.0f;
  f32 near = 0.1f;
  f32 far = 100.0f;
  f32 orthoSize = 5.0f;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_VIEWPORT_VIEWPORT_CONFIG_H_
