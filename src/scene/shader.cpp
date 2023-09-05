#include <glm/gtc/type_ptr.hpp>
#include <uinta/camera/target_camera.hpp>
#include <uinta/error.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/scene/light.hpp>
#include <uinta/scene/shader.hpp>
#include <uinta/shader.hpp>

namespace uinta {

uinta_error_code SceneShader::init(FileManager& fileManager) {
  const auto* vs = fileManager.registerFile("shader/scene.vs");
  const auto* fs = fileManager.registerFile("shader/scene.fs");
  fileManager.loadFile({vs, fs});
  const std::vector<std::string> screenSrcs = {fileManager.getDataString(vs), fileManager.getDataString(fs)};
  const std::vector<GLenum> screenStages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  const std::vector<std::string> uNames = {"u_lightColor", "u_lightDir", "u_model", "u_proj", "u_view", "u_time"};
  const std::vector<GLuint*> uLocations = {&u_lightColor, &u_lightDir, &u_model, &u_proj, &u_view, &u_time};
  if (auto error = createShaderProgram(id, screenSrcs, screenStages, uNames, uLocations); error) return error;
  fileManager.releaseFile({vs, fs});
  return SUCCESS_EC;
}

void SceneShader_OpenGL::start(const RunnerState& state, const glm::mat4& view, const glm::mat4& proj) const {
  glUseProgram(id);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(u_proj, 1, GL_FALSE, glm::value_ptr(proj));
  glUniform1f(u_time, state.runtime);
}

void SceneShader_OpenGL::diffuse(const Light& light) const {
  glUniform3fv(u_lightDir, 1, glm::value_ptr(light.direction));
  // TODO: Color and position
}

}  // namespace uinta
