#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <uinta/error.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene/scene.hpp>

namespace uinta {

enum class error {
  InvalidModelPath = 100,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::InvalidModelPath), "Model path is required for scene entities"},
};

UINTA_ERROR_FRAMEWORK(Scene, errorMessages);

inline glm::mat4 getTransform(const glm::mat4& baseTransform, const entt::entity entity, const entt::registry& registry);

Scene::Scene(Runner& runner)
    : fileManager(runner.fileManager),
      modelManager(runner.modelManager),
      diffuseLight({glm::normalize(glm::vec3(0, -3, 1)), {0, 0, 0}, {0, 0, 0}}) {
}

uinta_error_code Scene::init(Runner& runner) {
  if (auto error = shader.init(fileManager); error) return error;
  setFlag(CAMERA_ENABLED, isFlagSet(Runner::RENDERING_ENABLED, runner.flags), flags);
  camera.config.aspectRatio = runner.window.aspectRatio;
  return SUCCESS_EC;
}

void Scene::update(const RunnerState& state, const InputState& input, entt::registry& registry) {
  if (isFlagSet(CAMERA_ENABLED, flags)) updateCamera(camera, state, input);
}

uinta_error_code Scene::addEntity(entt::entity& ref, const SceneEntityInitializer& info, entt::registry& registry) {
  if (std::empty(info.modelPath)) return make_error(error::InvalidModelPath);
  const auto* file = fileManager.registerFile(info.modelPath);
  fileManager.loadFile(file);
  model_t model;
  if (auto error = modelManager.loadModel(model, file, fileManager); error) return error;
  if (auto error = addModel(model); error) return error;
  fileManager.releaseFile(file);

  ref = registry.create();
  registry.emplace<Model>(ref, modelManager.getModel(model));
  registry.emplace<Transform>(ref, info.transform);

  return SUCCESS_EC;
}

uinta_error_code Scene::addModel(const model_t model) {
  if (auto error = initVao(vao); error) return error;
  if (auto error = uploadVbo(vbo, modelManager.getVertexBuffer(model), modelManager.getVertexBufferSize(model)); error)
    return error;
  if (auto error = initVertexAttribs(vao); error) return error;
  if (auto error = indexBuffer(vao, modelManager.getIndexBuffer(model), modelManager.getIndexBufferSize(model)); error)
    return error;
  return SUCCESS_EC;
}

void Scene::startRender(const RunnerState& state) {
  const auto view = getViewMatrix(camera);
  const auto proj = getPerspectiveMatrix(camera);
  shader.start(view, proj, state);
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
