#include <spdlog/sinks/stdout_color_sinks.h>

#include <uinta/error.hpp>
#include <uinta/exception.hpp>
#include <uinta/logging.hpp>
#include <uinta/runner.hpp>
#include <uinta/scene.hpp>

namespace uinta {

enum class error {
  InvalidStateTransition = 100,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::InvalidStateTransition), "Invalid state transition!"},
};

UINTA_ERROR_FRAMEWORK(Scene, errorMessages);

inline uinta_error_code validate_transition(const Scene& scene, Scene::State state) noexcept;

Scene::Scene(const std::string& name, Runner& runner, Layer layer) noexcept
    : m_runner(runner),
      m_name(name),
      m_logger(spdlog::stdout_color_mt(m_runner.logger()->name() + ":Scene:" + m_name)),
      m_state(State::Created),
      m_layer(layer) {
  runner.add_event(RunnerEvents::SceneCreated, std::make_unique<SceneEvent>(runner.state().runtime, this));
}

Scene::~Scene() = default;

uinta_error_code Scene::transition(Scene::State state) noexcept {
  if (auto error = validate_transition(*this, state); error) return error;
  m_state = state;
  SPDLOG_LOGGER_INFO(m_logger, "Transitioned state to '{}'.", to_string(state));
  switch (state) {
    case Scene::State::Destroyed:
      on_destroyed();
      runner().add_event(RunnerEvents::SceneDestroyed, std::make_unique<SceneEvent>(runner().state().runtime, this));
      return SUCCESS_EC;
    case Scene::State::Paused:
      on_paused();
      runner().add_event(RunnerEvents::ScenePaused, std::make_unique<SceneEvent>(runner().state().runtime, this));
      return SUCCESS_EC;
    case Scene::State::Running:
      on_running();
      runner().add_event(RunnerEvents::SceneRunning, std::make_unique<SceneEvent>(runner().state().runtime, this));
      return SUCCESS_EC;
    default:
      return make_error(error::InvalidStateTransition);
  }
}

void Scene::shutdown() {
  if (auto error = transition(State::Destroyed); error) throw UintaException(error);
}

inline uinta_error_code validate_transition(const Scene& scene, Scene::State state) noexcept {
  if (Scene::State::Created == state) {  // Created is the initial state, and this can only be set internally.
    SPDLOG_LOGGER_ERROR(scene.logger(), "Cannot transition state to 'Created'.");
    return make_error(error::InvalidStateTransition);
  }
  if (Scene::State::Destroyed == scene.state()) {
    SPDLOG_LOGGER_ERROR(scene.logger(), "Cannot transition state after scene has been destroyed.");
    return make_error(error::InvalidStateTransition);
  }
  return SUCCESS_EC;
}

}  // namespace uinta
