#ifndef UINTA_SCENE_RENDERER_HPP
#define UINTA_SCENE_RENDERER_HPP

#include <entt/fwd.hpp>
#include <memory>
#include <uinta/scene/shader.hpp>

namespace uinta {

class SceneRenderer {
  friend class Scene;

 public:
  SceneRenderer(std::unique_ptr<SceneShader> shader = nullptr) : m_shader(std::move(shader)) {
    assert(m_shader && "Shader must be initialized!");
  }

  virtual uinta_error_code init(FileManager& fileManager) = 0;

  virtual void start(const glm::mat4& view, const glm::mat4& proj, const RunnerState& state) const = 0;

  virtual void renderEntity(const entt::entity entity, const entt::registry& registry) = 0;

  void diffuse(const Light& light) {
    m_shader->diffuse(light);
  };

  const SceneShader* shader() const noexcept {
    return m_shader.get();
  }

 protected:
  const std::unique_ptr<SceneShader> m_shader;
};

class SceneRenderer_OpenGL final : public SceneRenderer {
 public:
  SceneRenderer_OpenGL() : SceneRenderer(std::make_unique<SceneShader>()) {
  }

  uinta_error_code init(FileManager& fileManager) override;
  void start(const glm::mat4& view = glm::mat4(), const glm::mat4& proj = glm::mat4(),
             const RunnerState& state = {}) const override;
  void renderEntity(const entt::entity entity, const entt::registry& registry) override;

 protected:
};

}  // namespace uinta

#endif  // UINTA_SCENE_RENDERER_HPP
