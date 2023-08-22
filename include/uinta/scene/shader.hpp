#ifndef UINTA_SCENE_SHADER_HPP
#define UINTA_SCENE_SHADER_HPP

#include <uinta/gl/api.h>

#include <glm/mat4x4.hpp>
#include <uinta/runner/runner_state.hpp>

namespace uinta {

class FileManager;
struct Light;

class SceneShader {
 public:
  GLuint id;
  GLuint u_lightColor;
  GLuint u_lightDir;
  GLuint u_model;
  GLuint u_proj;
  GLuint u_view;
  GLuint u_time;

  bool init(FileManager& fileManager);

  void start(const glm::mat4& view = glm::mat4(), const glm::mat4& proj = glm::mat4(), const RunnerState& state = {}) const;

  void updateDiffuseLight(const Light& light) const;
};

}  // namespace uinta

#endif  // UINTA_SCENE_SHADER_HPP
