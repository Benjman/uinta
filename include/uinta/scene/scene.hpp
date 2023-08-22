#ifndef UINTA_SCENE_HPP
#define UINTA_SCENE_HPP

#include <uinta/components.hpp>
#include <uinta/gl/vao.hpp>
#include <uinta/gl/vbo.hpp>
#include <uinta/model_manager.hpp>
#include <uinta/scene/light.hpp>
#include <uinta/scene/shader.hpp>

namespace uinta {

struct SceneEntityInitializer {
  std::string modelPath;
  Transform transform = {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}};
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

  void update(const RunnerState& state, const InputState& input, entt::registry& registry);

  entt::entity addEntity(const SceneEntityInitializer& info, entt::registry& registry);

  void addModel(const model_t model);

  void startRender(const Runner* runner, const RunnerState& state);

  void renderEntity(const entt::entity entity, const entt::registry& registry);

  const Light& getDiffuseLight() const;

  void updateDiffuseLight(const Light& light);

 private:
  flags_t flags = DIFFUSE_LIGHT_DIRTY;
  FileManager* fileManager;
  ModelManager* modelManager;
  Light diffuseLight;
};

}  // namespace uinta

#endif  // UINTA_SCENE_HPP
