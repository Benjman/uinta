#include <uinta/gl/api.h>

#include <entt/entity/registry.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <uinta/component/model.hpp>
#include <uinta/component/transform.hpp>
#include <uinta/scene/renderer.hpp>

namespace uinta {

inline glm::mat4 transform(const glm::mat4& baseTransform, const Transform& transform) {
  auto model = glm::translate(baseTransform, transform.position);
  if (transform.rotation.x) model = glm::rotate(model, transform.rotation.x, glm::vec3(1, 0, 0));
  if (transform.rotation.y) model = glm::rotate(model, transform.rotation.y, glm::vec3(0, 1, 0));
  if (transform.rotation.z) model = glm::rotate(model, transform.rotation.z, glm::vec3(0, 0, 1));
  model = glm::scale(model, transform.scale);
  return model;
}

void SceneRenderer_OpenGL::renderEntity(const entt::entity entity, const entt::registry& registry) {
  const auto& model = registry.get<Model>(entity);
  glUniformMatrix4fv(m_shader->u_model, 1, false, glm::value_ptr(transform(glm::mat4(1), registry.get<Transform>(entity))));
  glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLfloat) * 0));
}

}  // namespace uinta
