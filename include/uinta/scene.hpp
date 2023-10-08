#ifndef UINTA_SCENE_HPP
#define UINTA_SCENE_HPP

#include <spdlog/fwd.h>

#include <memory>
#include <string>
#include <uinta/fwd.hpp>
#include <uinta/runner_lifecycle.hpp>

namespace uinta {

class Scene : public RunnerLifecycle {
 public:
  enum class Layer : u8 {
    Simulation = 0,
    UI = 1,
    Debug = 2,
  };

  enum class State : u8 {
    Created,
    Running,
    Paused,
    Destroyed,
  };

  Scene(const std::string& name, Runner& runner, Layer layer) noexcept;

  virtual ~Scene();

  uinta_error_code init() override = 0;

  void pre_tick(const RunnerState& state, const InputState& input) override {
  }

  void tick(const RunnerState& state, const InputState& input) override {
  }

  void post_tick(const RunnerState& state, const InputState& input) override {
  }

  void pre_render(const RunnerState& state) override {
  }

  void render(const RunnerState& state) override {
  }

  void post_render(const RunnerState& state) override {
  }

  void shutdown() override;

  virtual const TargetCamera* camerac() noexcept {
    return camera();
  }

  virtual TargetCamera* camera() noexcept {
    return nullptr;
  }

  uinta_error_code transition(Scene::State new_state) noexcept;

  State state() const noexcept {
    return m_state;
  }

  std::string name() const noexcept {
    return m_name;
  }

  Layer layer() const noexcept {
    return m_layer;
  }

  spdlog::logger* logger() const noexcept {
    return m_logger.get();
  }

  Runner& runner() const noexcept {
    return m_runner;
  }

 protected:
  virtual void on_destroyed() noexcept {
  }

  virtual void on_initialized() noexcept {
  }

  virtual void on_paused() noexcept {
  }

  virtual void on_running() noexcept {
  }

 private:
  Runner& m_runner;
  std::string m_name;
  std::shared_ptr<spdlog::logger> m_logger;
  State m_state;
  Layer m_layer;
};

inline std::string to_string(Scene::Layer layer) {
  switch (layer) {
    case Scene::Layer::Simulation:
      return "Simulation";
    case Scene::Layer::UI:
      return "UI";
    case Scene::Layer::Debug:
      return "Debug";
    default:
      return "Unknown";
  }
}

inline std::string to_string(Scene::State state) {
  switch (state) {
    case Scene::State::Created:
      return "Created";
    case Scene::State::Running:
      return "Running";
    case Scene::State::Paused:
      return "Paused";
    case Scene::State::Destroyed:
      return "Destroyed";
    default:
      return "Unknown";
  }
}

}  // namespace uinta

#endif  // UINTA_SCENE_HPP
