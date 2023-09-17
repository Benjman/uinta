#ifndef UINTA_SCENE_HPP
#define UINTA_SCENE_HPP

#include <spdlog/fwd.h>
#include <uinta/flags.h>
#include <uinta/types.h>

#include <entt/entity/registry.hpp>
#include <uinta/scene/light.hpp>
#include <uinta/target_camera.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

class Runner;

class Scene {
 public:
  static constexpr flag_t DIFFUSE_LIGHT_DIRTY = 1 << 0;
  static constexpr flag_t CAMERA_ENABLED = 1 << 1;
  static constexpr flag_t GRID_ENABLED = 1 << 2;

  Scene(Runner& runner);

  uinta_error_code init();

  virtual void preTick(const RunnerState& state, const InputState& input);

  virtual void tick(const RunnerState& state, const InputState& input);

  virtual void postTick(const RunnerState& state, const InputState& input);

  const Runner& runner() const noexcept {
    return m_runner;
  }

  void render(const RunnerState& state);

  void onAspectRatioUpdate(f32 newAspectRatio);

  const TargetCamera& camera() const noexcept {
    return m_camera;
  }

  void camera(const TargetCamera& camera) noexcept {
    m_camera = camera;
  }

  const Light& diffuse_light() const noexcept {
    return m_diffuse_light;
  }

  void diffuse_light(const Light& v) noexcept {
    m_diffuse_light = v;
    setFlag(DIFFUSE_LIGHT_DIRTY, true, m_flags);
  }

  void flag(const flag_t mask, const bool state) {
    setFlag(mask, state, m_flags);
  }

  const flags_t flags() const noexcept {
    return m_flags;
  }

  void flags(const flags_t v) noexcept {
    m_flags = v;
  }

  const CartesianGrid& cartesian_grid() const noexcept {
    return m_cartesian_grid;
  }

  entt::registry& registry() {
    return m_registry;
  }

 private:
  Runner& m_runner;
  CartesianGrid m_cartesian_grid;
  TargetCamera m_camera;
  Light m_diffuse_light;
  std::shared_ptr<spdlog::logger> m_logger;
  entt::registry m_registry;
  flags_t m_flags = DIFFUSE_LIGHT_DIRTY | CAMERA_ENABLED | GRID_ENABLED;
};

}  // namespace uinta

#endif  // UINTA_SCENE_HPP
