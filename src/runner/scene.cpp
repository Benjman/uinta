#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <uinta/components.hpp>
#include <uinta/file_manager.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/runner/scene.hpp>
#include <uinta/shader.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

inline glm::mat4 getTransform(glm::mat4 baseTransform, const entt::entity entity, const entt::registry& registry);

Scene::Scene() : diffuseLighting({glm::normalize(glm::vec3(-2, 3, 1))}) {
}

bool Scene::init(Runner* runner) {
  fileManager = &runner->fileManager;
  modelManager = &runner->modelManager;
  registry = &runner->registry;
  if (!shader.init(*fileManager)) return false;
  return true;
}

entt::entity Scene::addEntity(const SceneEntityInitializer& info) {
  auto* file = fileManager->registerFile(info.modelPath);
  fileManager->loadFile(file);
  auto model = modelManager->loadModel(file, fileManager);
  addModel(model);
  fileManager->releaseFile(file);

  Transform transform = {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}};
  entt::entity result = registry->create();
  registry->emplace<Model>(result, modelManager->getModel(model));
  registry->emplace<Transform>(result, transform);

  return result;
}

void Scene::addModel(const model_t model) {
  initVao(vao);
  uploadVbo(vbo, modelManager->getVertexBuffer(model), modelManager->getVertexBufferSize(model));
  initVertexAttribs(vao);
  indexBuffer(vao, modelManager->getIndexBuffer(model), modelManager->getIndexBufferSize(model));
}

void Scene::startRender(const Runner* runner, const RunnerState& state) {
  shader.start(runner, state);
  bindVao(vao);
}

void Scene::render(const entt::entity entity, const entt::registry& registry) {
  auto& transform = registry.get<Transform>(entity);
  glUniformMatrix4fv(shader.u_model, 1, false, glm::value_ptr(getTransform(glm::mat4(1), entity, registry)));
  auto& model = registry.get<Model>(entity);
  glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0));
}

bool SceneShader::init(FileManager& fileManager) {
  auto vs = fileManager.registerFile("shader/scene.vs");
  auto fs = fileManager.registerFile("shader/scene.fs");
  fileManager.loadFile({vs, fs});
  std::vector<std::string> screenSrcs({fileManager.getDataString(vs), fileManager.getDataString(fs)});
  std::vector<GLenum> screenStages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
  std::vector<std::string> uNames({"u_lightColor", "u_lightDir", "u_model", "u_proj", "u_view", "u_time"});
  std::vector<GLuint*> uLocations = {&u_lightColor, &u_lightDir, &u_model, &u_proj, &u_view, &u_time};
  id = createShaderProgram(screenSrcs, screenStages, uNames, uLocations);
  fileManager.releaseFile({vs, fs});
  return id > GL_ZERO;
}

void SceneShader::start(const Runner* runner, const RunnerState& state) const {
  glUseProgram(id);
  glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(getViewMatrix(runner->camera)));
  glUniformMatrix4fv(u_proj, 1, GL_FALSE, glm::value_ptr(getPerspectiveMatrix(runner->camera, runner->display)));
  glUniform3fv(u_lightDir, 1, glm::value_ptr(runner->scene.diffuseLighting.direction));
  glUniform1f(u_time, state.runtime);
}

inline glm::mat4 getTransform(glm::mat4 baseTransform, const entt::entity entity, const entt::registry& registry) {
  auto& transform = registry.get<Transform>(entity);
  auto mvp = glm::translate(baseTransform, transform.position);
  mvp = glm::scale(mvp, transform.scale);
  if (transform.rotation.x) mvp = glm::rotate(mvp, transform.rotation.x, glm::vec3(1, 0, 0));
  if (transform.rotation.y) mvp = glm::rotate(mvp, transform.rotation.y, glm::vec3(0, 1, 0));
  if (transform.rotation.z) mvp = glm::rotate(mvp, transform.rotation.z, glm::vec3(0, 0, 1));
  return mvp;
}

}  // namespace uinta