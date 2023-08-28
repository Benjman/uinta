#ifndef UINTA_APP_POST_PROCESSING_SHADERS_HPP
#define UINTA_APP_POST_PROCESSING_SHADERS_HPP

#include <uinta/file_manager.hpp>
#include <uinta/shader.hpp>

namespace uinta {

class ColorShader {
 public:
  uinta_error_code init(FileManager& fm) {
    const auto vs = fm.registerFile("shader/base.vs");
    const auto fs = fm.registerFile("shader/color.fs");
    fm.loadFile({vs, fs});
    const std::vector<std::string> srcs = {fm.getDataString(vs), fm.getDataString(fs)};
    const std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    const std::vector<std::string> uNames = {"mvp"};
    const std::vector<GLuint*> uLocs = {&u_mvp};
    if (auto error = createShaderProgram(shaderId, srcs, stages, uNames, uLocs); error) return error;
    return SUCCESS_EC;
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

  uinta_error_code init(FileManager& fm) {
    auto vs = fm.registerFile("shader/screen.vs");
    auto fs = fm.registerFile("shader/shimmer.fs");
    fm.loadAll();
    auto screenVertSrc = fm.getDataString(vs);
    auto screenFragSrc = fm.getDataString(fs);
    const std::vector<std::string> screenSrcs({screenVertSrc, screenFragSrc});
    const std::vector<GLenum> screenStages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> screenUniforms({"renderedTexture", "time", "scale"});
    const std::vector<GLuint*> screenLocations = {&u_screen, &u_time, &u_scale};
    if (auto error = createShaderProgram(screen, screenSrcs, screenStages, screenUniforms, screenLocations); error) return error;
    return SUCCESS_EC;
  }
};

}  // namespace uinta

#endif  // UINTA_APP_POST_PROCESSING_SHADERS_HPP
