#ifndef UINTA_SCENE_HPP
#define UINTA_SCENE_HPP

#include <entt/fwd.hpp>
#include <glm/vec3.hpp>
#include <uinta/components.hpp>
#include <uinta/fwd.hpp>
#include <uinta/gl/api.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>

namespace uinta {

struct SceneEntityInitializer {
  std::string modelPath;
  Transform transform;
};

struct Light {
  glm::vec3 direction;
  glm::vec3 pos;
  glm::vec3 color;
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
};

class Scene {
 public:
  Vao vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 3 * sizeof(float)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 6 * sizeof(float)},
  }};
  Vbo vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  SceneShader shader;

  Light diffuseLighting;

  Scene();

  bool init(Runner* runner);

  entt::entity addEntity(const SceneEntityInitializer& info);
  void addModel(const model_t model);

  void startRender(const Runner* runner, const RunnerState& state);
  void render(const entt::entity entity, const entt::registry& registry);

 private:
  FileManager* fileManager;
  ModelManager* modelManager;
  entt::registry* registry;
};

}  // namespace uinta

#endif  // UINTA_SCENE_HPP
