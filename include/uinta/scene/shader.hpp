#ifndef UINTA_SCENE_SHADER_HPP
#define UINTA_SCENE_SHADER_HPP

#include <uinta/gl/api.h>

#include <uinta/fwd.hpp>
#include <uinta/scene/light.hpp>

namespace uinta {

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

}  // namespace uinta

#endif  // UINTA_SCENE_SHADER_HPP
