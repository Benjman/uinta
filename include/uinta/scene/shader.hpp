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

  virtual uinta_error_code init(FileManager& fileManager);
  virtual void start(const RunnerState& state, const glm::mat4& view, const glm::mat4& proj) const = 0;
  virtual void diffuse(const Light& light) const = 0;
};

class SceneShader_OpenGL final : public SceneShader {
 public:
  void start(const RunnerState& state = {}, const glm::mat4& view = {}, const glm::mat4& proj = {}) const override;
  void diffuse(const Light& light) const override;
};

}  // namespace uinta

#endif  // UINTA_SCENE_SHADER_HPP
