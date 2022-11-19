#ifndef UINTA_APP_POST_PROCESSING_SHADERS_HPP
#define UINTA_APP_POST_PROCESSING_SHADERS_HPP

#include <uinta/file_manager.hpp>
#include <uinta/shader.hpp>

namespace uinta {

class ColorShader {
 public:
  bool init(FileManager& fm) {
    auto vs = fm.registerFile("shader/base.vs");
    auto fs = fm.registerFile("shader/color.fs");
    fm.loadFile({vs, fs});

    shaderId = createShaderProgram({fm.getDataString(vs), fm.getDataString(fs)}, {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"mvp"},
                                   {&u_mvp});

    return true;
  }

  uint shaderId;
  uint u_mvp;
};

struct PostProcessingShaders {
  uint shaderColor;

  GLuint screen;

  const file_t* f_scene_v;
  const file_t* f_scene_f;
  const file_t* f_screen_v;
  const file_t* f_screen_f;

  GLuint u_screen, u_time, u_scale;

  float shimmerScale = 0.001;

  void init(FileManager& fm) {
    auto vs = fm.registerFile("shader/screen.vs");
    auto fs = fm.registerFile("shader/shimmer.fs");
    fm.loadAll();
    auto screenVertSrc = fm.getDataString(vs);
    auto screenFragSrc = fm.getDataString(fs);
    const std::vector<std::string> screenSrcs({screenVertSrc, screenFragSrc});
    const std::vector<GLenum> screenStages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> screenUniforms({"renderedTexture", "time", "scale"});
    const std::vector<GLuint*> screenLocations = {&u_screen, &u_time, &u_scale};
    screen = createShaderProgram(screenSrcs, screenStages, screenUniforms, screenLocations);
  }
};

}  // namespace uinta

#endif  // UINTA_APP_POST_PROCESSING_SHADERS_HPP
