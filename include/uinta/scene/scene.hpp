#ifndef UINTA_SCENE_HPP
#define UINTA_SCENE_HPP

#include <uinta/gl/api.h>

#include <entt/fwd.hpp>
#include <uinta/components.hpp>
#include <uinta/fwd.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/model_manager.hpp>

namespace uinta {

struct SceneEntityInitializer {
  std::string modelPath;
  Transform transform = {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}};
};

struct Light {
  glm::vec3 direction;
  glm::vec3 pos;
  glm::vec3 color;

  Light(const glm::vec3& direction, const glm::vec3& pos = {0, 0, 0}, const glm::vec3& color = {0, 0, 0}) noexcept;

  Light(const Light& other) noexcept;

  Light& operator=(const Light& other) noexcept;
};

class SceneShader {
 public:
  GLuint id;
  GLuint u_lightColor;
  GLuint u_lightDir;
  GLuint u_model;
  GLuint u_proj;
  GLuint u_view;
  GLuint u_time;

  bool init(FileManager& fileManager);

  void start(const Runner* runner, const RunnerState& state) const;

  void updateDiffuseLight(const Light& light) const;
};

class Scene {
  static constexpr flag_t DIFFUSE_LIGHT_DIRTY = 1 << 0;

 public:
  Vao vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 3 * sizeof(f32)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), 6 * sizeof(f32)},
  }};
  Vbo vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  SceneShader shader;

  Scene();

  bool init(Runner* runner);
  void onTick(const RunnerState& state);

  entt::entity addEntity(const SceneEntityInitializer& info);
  void addModel(const model_t model);

  void startRender(const Runner* runner, const RunnerState& state);
  void render(const entt::entity entity, const entt::registry& registry);

  const Light& getDiffuseLight() const;

  void updateDiffuseLight(const Light& light);

 private:
  flags_t flags = DIFFUSE_LIGHT_DIRTY;
  FileManager* fileManager;
  ModelManager* modelManager;
  entt::registry* registry;
  Light diffuseLight;
};

}  // namespace uinta

#endif  // UINTA_SCENE_HPP
