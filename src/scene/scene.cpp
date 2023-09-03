#include <entt/entt.hpp>
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

Scene::Scene(entt::registry* const registry, std::unique_ptr<SceneRenderer> renderer)
    : m_diffuse_light({glm::normalize(glm::vec3(0, -3, 1))}),
      m_renderer(renderer ? std::move(renderer) : std::make_unique<SceneRenderer_OpenGL>()) {
  assert(m_renderer && "Renderer must be initialized!");
}

uinta_error_code Scene::init(Runner& runner) {
  if (auto error = m_renderer->init(runner.file_manager()); error) return error;
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
  m_renderer->start(getViewMatrix(m_camera), getPerspectiveMatrix(m_camera), state);
  if (isFlagSet(DIFFUSE_LIGHT_DIRTY, m_flags)) {
    m_renderer->diffuse(m_diffuse_light);
    setFlag(DIFFUSE_LIGHT_DIRTY, false, m_flags);
  }
  bindVao(m_vao);
}

void Scene::renderEntity(const entt::entity entity, const entt::registry& registry) {
  m_renderer->renderEntity(entity, registry);
}

void Scene::onAspectRatioUpdate(f32 aspect_ratio) {
  m_camera.aspect_ratio(aspect_ratio);
}

}  // namespace uinta
