#include <uinta/gl/api.h>

#include <entt/entity/registry.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <uinta/component/model.hpp>
#include <uinta/component/transform.hpp>
#include <uinta/scene/renderer.hpp>

namespace uinta {

inline glm::mat4 getTransform(const glm::mat4& baseTransform, const entt::entity entity, const entt::registry& registry) {
  auto& transform = registry.get<Transform>(entity);
  auto model = glm::translate(baseTransform, transform.position);
  if (transform.rotation.x) model = glm::rotate(model, transform.rotation.x, glm::vec3(1, 0, 0));
  if (transform.rotation.y) model = glm::rotate(model, transform.rotation.y, glm::vec3(0, 1, 0));
  if (transform.rotation.z) model = glm::rotate(model, transform.rotation.z, glm::vec3(0, 0, 1));
  model = glm::scale(model, transform.scale);
  return model;
}

uinta_error_code SceneRenderer_OpenGL::init(FileManager& fileManager) {
  if (auto error = m_shader.get()->init(fileManager); error) return error;
  return SUCCESS_EC;
}

void SceneRenderer_OpenGL::start(const glm::mat4& view, const glm::mat4& proj, const RunnerState& state) const {
  m_shader->start(view, proj, state);
}

void SceneRenderer_OpenGL::renderEntity(const entt::entity entity, const entt::registry& registry) {
  glUniformMatrix4fv(m_shader->u_model, 1, false, glm::value_ptr(getTransform(glm::mat4(1), entity, registry)));
  auto& model = registry.get<Model>(entity);
  glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0));
}

}  // namespace uinta
