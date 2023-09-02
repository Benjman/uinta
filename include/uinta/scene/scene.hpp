#ifndef UINTA_SCENE_HPP
#define UINTA_SCENE_HPP

#include <uinta/flags.h>
#include <uinta/types.h>

#include <entt/fwd.hpp>
#include <string>
#include <uinta/camera/target_camera.hpp>
#include <uinta/component/transform.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/scene/light.hpp>
#include <uinta/scene/shader.hpp>
#include <uinta/utils/cartesian_grid.hpp>

namespace uinta {

class Runner;

struct SceneEntityInitializer {
  std::string modelPath;
  Transform transform = {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}};
};

class Scene {
 public:
  static constexpr flag_t DIFFUSE_LIGHT_DIRTY = 1 << 0;
  static constexpr flag_t CAMERA_ENABLED = 1 << 1;
  static constexpr flag_t GRID_ENABLED = 1 << 2;

  Scene(entt::registry* const registry);

  uinta_error_code init(Runner& runner);

  void update(const RunnerState& state, const InputState& input, entt::registry& registry);

  uinta_error_code addEntity(entt::entity& ref, FileManager& file_manager, ModelManager& model_manager,
                             const SceneEntityInitializer& info, entt::registry& registry);

  uinta_error_code addModel(const model_t model, ModelManager& model_manager);

  void render(const RunnerState& state);

  void renderEntity(const entt::entity entity, const entt::registry& registry);

  void onAspectRatioUpdate(f32 newAspectRatio);

  const Vao& vao() const noexcept {
    return m_vao;
  }

  const Vbo& vbo() const noexcept {
    return m_vbo;
  }

  const SceneShader& shader() const noexcept {
    return m_shader;
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

 private:
  Vao m_vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 3 * sizeof(f32)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 6 * sizeof(f32)},
  }};
  Vbo m_vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  CartesianGrid m_cartesian_grid;
  SceneShader m_shader;
  TargetCamera m_camera;
  Light m_diffuse_light;
  flags_t m_flags = DIFFUSE_LIGHT_DIRTY | CAMERA_ENABLED | GRID_ENABLED;
};

}  // namespace uinta

#endif  // UINTA_SCENE_HPP
