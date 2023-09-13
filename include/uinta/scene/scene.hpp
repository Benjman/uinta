#ifndef UINTA_SCENE_HPP
#define UINTA_SCENE_HPP

#include <uinta/flags.h>
#include <uinta/types.h>

#include <entt/entity/registry.hpp>
#include <string>
#include <uinta/component/transform.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/scene/light.hpp>
#include <uinta/scene/renderer.hpp>
#include <uinta/target_camera.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

class Runner;

struct SceneEntityInitializer {
  std::string modelPath;
  Transform transform = {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}};
};

struct SceneDependencies {
  std::unique_ptr<SceneRenderer> renderer = std::make_unique<SceneRenderer_OpenGL>();
};

class Scene {
 public:
  static constexpr flag_t DIFFUSE_LIGHT_DIRTY = 1 << 0;
  static constexpr flag_t CAMERA_ENABLED = 1 << 1;
  static constexpr flag_t GRID_ENABLED = 1 << 2;

  Scene(Runner& runner, SceneDependencies dependencies);

  uinta_error_code init();

  virtual void preTick(const RunnerState& state, const InputState& input);

  virtual void tick(const RunnerState& state, const InputState& input);

  virtual void postTick(const RunnerState& state, const InputState& input);

  uinta_error_code addEntity(entt::entity& ref, FileManager& file_manager, ModelManager& model_manager,
                             const SceneEntityInitializer& info);

  uinta_error_code addModel(const model_t model, ModelManager& model_manager);

  const Runner& runner() const noexcept {
    return m_runner;
  }

  const SceneRenderer& renderer() {
    return *m_renderer;
  }

  void render(const RunnerState& state);

  void renderEntity(const entt::entity entity);

  void onAspectRatioUpdate(f32 newAspectRatio);

  const Vao& vao() const noexcept {
    return m_vao;
  }

  const Vbo& vbo() const noexcept {
    return m_vbo;
  }

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
  Vao m_vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 3 * sizeof(f32)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 6 * sizeof(f32)},
  }};
  Runner& m_runner;
  Vbo m_vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  CartesianGrid m_cartesian_grid;
  TargetCamera m_camera;
  Light m_diffuse_light;
  std::unique_ptr<SceneRenderer> m_renderer;
  entt::registry m_registry;
  flags_t m_flags = DIFFUSE_LIGHT_DIRTY | CAMERA_ENABLED | GRID_ENABLED;
};

}  // namespace uinta

#endif  // UINTA_SCENE_HPP
