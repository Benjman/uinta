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

  virtual uinta_error_code init(FileManager& fileManager) {
    if (auto error = m_shader.get()->init(fileManager); error) return error;
    return SUCCESS_EC;
  }

  virtual void start(const RunnerState& state, const glm::mat4& view, const glm::mat4& proj) const {
    m_shader->start(state, view, proj);
  }

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
  SceneRenderer_OpenGL() : SceneRenderer(std::make_unique<SceneShader_OpenGL>()) {
  }

  void renderEntity(const entt::entity entity, const entt::registry& registry) override;
};

}  // namespace uinta

#endif  // UINTA_SCENE_RENDERER_HPP
