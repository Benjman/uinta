#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <uinta/components.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/runner/runner_state.hpp>
#include <uinta/scene/scene.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

inline glm::mat4 getTransform(const glm::mat4& baseTransform, const entt::entity entity, const entt::registry& registry);

Scene::Scene() : diffuseLight({glm::normalize(glm::vec3(0, -3, 1)), {0, 0, 0}, {0, 0, 0}}) {
}

bool Scene::init(Runner* runner) {
  fileManager = &runner->fileManager;
  modelManager = &runner->modelManager;
  registry = &runner->registry;
  if (!shader.init(*fileManager)) return false;
  return true;
}

void Scene::onTick(const RunnerState& state) {
}

entt::entity Scene::addEntity(const SceneEntityInitializer& info) {
  if (std::empty(info.modelPath)) throw std::invalid_argument("Model path is required for scene entities");
  auto* file = fileManager->registerFile(info.modelPath);
  fileManager->loadFile(file);
  auto model = modelManager->loadModel(file, fileManager);
  addModel(model);
  fileManager->releaseFile(file);

  entt::entity result = registry->create();
  registry->emplace<Model>(result, modelManager->getModel(model));
  registry->emplace<Transform>(result, info.transform);

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

void Scene::render(const entt::entity entity, const entt::registry& registry) {
  // TODO: Generate transformation matrix: auto& transform = registry.get<Transform>(entity);
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
