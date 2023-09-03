#ifndef UINTA_SCENE_SHADER_HPP
#define UINTA_SCENE_SHADER_HPP

#include <glm/mat4x4.hpp>
#include <uinta/error.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

class FileManager;
struct Light;

class SceneShader {
 public:
  u32 id;
  u32 u_lightColor;
  u32 u_lightDir;
  u32 u_model;
  u32 u_proj;
  u32 u_view;
  u32 u_time;

  uinta_error_code init(FileManager& fileManager);

  void start(const glm::mat4& view = glm::mat4(), const glm::mat4& proj = glm::mat4(), const RunnerState& state = {}) const;

  void diffuse(const Light& light) const;
};

}  // namespace uinta

#endif  // UINTA_SCENE_SHADER_HPP
