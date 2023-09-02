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

Scene::Scene(entt::registry* registry) : m_diffuse_light({glm::normalize(glm::vec3(0, -3, 1)), {0, 0, 0}, {0, 0, 0}}) {
}

uinta_error_code Scene::init(Runner& runner) {
  if (auto error = m_shader.init(runner.file_manager()); error) return error;
  if (auto error = m_cartesian_grid.init(runner.file_manager()); error) return error;
  setFlag(CAMERA_ENABLED, isFlagSet(Runner::RENDERING_ENABLED, runner.flags()), m_flags);
  m_camera.config.aspectRatio = runner.window().aspectRatio;
  return SUCCESS_EC;
}

void Scene::update(const RunnerState& state, const InputState& input, entt::registry& registry) {
  if (isFlagSet(CAMERA_ENABLED, m_flags)) updateCamera(m_camera, state, input);
}

uinta_error_code Scene::addEntity(entt::entity& ref, FileManager& file_manager, ModelManager& model_manager,
                                  const SceneEntityInitializer& info, entt::registry& registry) {
  if (std::empty(info.modelPath)) return make_error(error::InvalidModelPath);
  const auto* file = file_manager.registerFile(info.modelPath);
  file_manager.loadFile(file);
  model_t model;
  if (auto error = model_manager.load(model, file, file_manager); error) return error;
  if (auto error = addModel(model, model_manager); error) return error;
  file_manager.releaseFile(file);

  ref = registry.create();
  registry.emplace<Model>(ref, model_manager.model(model));
  registry.emplace<Transform>(ref, info.transform);

  return SUCCESS_EC;
}

uinta_error_code Scene::addModel(const model_t model, ModelManager& model_manager) {
  if (auto error = initVao(m_vao); error) return error;
  if (auto error = uploadVbo(m_vbo, model_manager.vertexBuffer(model), model_manager.vertexBufferSize(model)); error)
    return error;
  if (auto error = initVertexAttribs(m_vao); error) return error;
  if (auto error = indexBuffer(m_vao, model_manager.indexBuffer(model), model_manager.indexBufferSize(model)); error)
    return error;
  return SUCCESS_EC;
}

void Scene::render(const RunnerState& state) {
  if (isFlagSet(GRID_ENABLED, m_flags)) m_cartesian_grid.render(getPerspectiveMatrix(m_camera) * getViewMatrix(m_camera));
  m_shader.start(getViewMatrix(m_camera), getPerspectiveMatrix(m_camera), state);
  if (isFlagSet(DIFFUSE_LIGHT_DIRTY, m_flags)) {
    m_shader.updateDiffuseLight(m_diffuse_light);
    setFlag(DIFFUSE_LIGHT_DIRTY, false, m_flags);
  }
  bindVao(m_vao);
}

void Scene::renderEntity(const entt::entity entity, const entt::registry& registry) {
  glUniformMatrix4fv(m_shader.u_model, 1, false, glm::value_ptr(getTransform(glm::mat4(1), entity, registry)));
  auto& model = registry.get<Model>(entity);
  glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0));
}

void Scene::onAspectRatioUpdate(f32 aspect_ratio) {
  m_camera.aspect_ratio(aspect_ratio);
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
