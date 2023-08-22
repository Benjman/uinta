#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene/scene.hpp>

namespace uinta {

inline glm::mat4 getTransform(const glm::mat4& baseTransform, const entt::entity entity, const entt::registry& registry);

Scene::Scene() : diffuseLight({glm::normalize(glm::vec3(0, -3, 1)), {0, 0, 0}, {0, 0, 0}}) {
}

bool Scene::init(Runner* runner) {
  fileManager = &runner->fileManager;
  modelManager = &runner->modelManager;
  if (!shader.init(*fileManager)) return false;
  setFlag(CAMERA_ENABLED, isFlagSet(RUNNER_FLAG_RENDERING, runner->flags), flags);
  return true;
}

void Scene::update(const RunnerState& state, const InputState& input, entt::registry& registry) {
  if (isFlagSet(CAMERA_ENABLED, flags)) updateCamera(camera, state, input);
}

entt::entity Scene::addEntity(const SceneEntityInitializer& info, entt::registry& registry) {
  if (std::empty(info.modelPath)) throw std::invalid_argument("Model path is required for scene entities");
  auto* file = fileManager->registerFile(info.modelPath);
  fileManager->loadFile(file);
  auto model = modelManager->loadModel(file, fileManager);
  addModel(model);
  fileManager->releaseFile(file);

  entt::entity result = registry.create();
  registry.emplace<Model>(result, modelManager->getModel(model));
  registry.emplace<Transform>(result, info.transform);

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
  if (isFlagSet(DIFFUSE_LIGHT_DIRTY, flags)) {
    shader.updateDiffuseLight(diffuseLight);
    setFlag(DIFFUSE_LIGHT_DIRTY, false, flags);
  }
  bindVao(vao);
}

void Scene::renderEntity(const entt::entity entity, const entt::registry& registry) {
  glUniformMatrix4fv(shader.u_model, 1, false, glm::value_ptr(getTransform(glm::mat4(1), entity, registry)));
  auto& model = registry.get<Model>(entity);
  glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0));
}

const Light& Scene::getDiffuseLight() const {
  return diffuseLight;
}

void Scene::updateDiffuseLight(const Light& light) {
  diffuseLight = light;
  setFlag(DIFFUSE_LIGHT_DIRTY, true, flags);
}

inline glm::mat4 getTransform(const glm::mat4& baseTransform, const entt::entity entity, const entt::registry& registry) {
  auto& transform = registry.get<Transform>(entity);
  auto model = glm::translate(baseTransform, transform.position);
  if (transform.rotation.x) model = glm::rotate(model, transform.rotation.x, glm::vec3(1, 0, 0));
  if (transform.rotation.y) model = glm::rotate(model, transform.rotation.y, glm::vec3(0, 1, 0));
  if (transform.rotation.z) model = glm::rotate(model, transform.rotation.z, glm::vec3(0, 0, 1));
  model = glm::scale(model, transform.scale);
  return model;
}

}  // namespace uinta
