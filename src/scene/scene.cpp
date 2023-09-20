#include <spdlog/sinks/stdout_color_sinks.h>

#include <entt/entt.hpp>
#include <glm/mat4x4.hpp>
#include <uinta/error.hpp>
#include <uinta/grid.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene/scene.hpp>

namespace uinta {

Scene::Scene(Runner& runner)
    : m_runner(runner),
      m_diffuse_light({glm::normalize(glm::vec3(0, -3, 1))}),
      m_grid(std::make_unique<Grid>(*this)),
      m_logger(spdlog::stdout_color_st(m_runner.logger()->name() + ":Scene")) {
}

uinta_error_code Scene::init() {
  if (auto error = m_grid->init(m_runner.file_manager()); error) return error;
  setFlag(CAMERA_ENABLED, isFlagSet(Runner::RENDERING_ENABLED, m_runner.flags()), m_flags);
  m_camera.aspect_ratio(m_runner.window().aspect_ratio);
  SPDLOG_LOGGER_INFO(m_logger, "Initialized Scene.");
  return SUCCESS_EC;
}

Scene::~Scene() = default;

void Scene::preTick(const RunnerState& state, const InputState& input) {
}

void Scene::tick(const RunnerState& state, const InputState& input) {
  if (isFlagSet(CAMERA_ENABLED, m_flags)) m_camera.update(state, input);
}

void Scene::postTick(const RunnerState& state, const InputState& input) {
}

void Scene::render(const RunnerState& state) {
  glm::mat4 view = m_camera.view_matrix();
  if (isFlagSet(GRID_ENABLED, m_flags)) m_grid->render(m_camera.perspective_matrix() * m_camera.view_matrix());
}

void Scene::onAspectRatioUpdate(f32 aspect_ratio) {
  m_camera.aspect_ratio(aspect_ratio);
}

}  // namespace uinta
