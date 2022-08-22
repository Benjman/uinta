#ifndef UINTA_APP_POST_PROCESSING_SHADERS_HPP
#define UINTA_APP_POST_PROCESSING_SHADERS_HPP

#include <uinta/gl.h>

#include <uinta/io/file_manager.hpp>
#include <uinta/shader.hpp>

#include "./PostProcessingCamera.hpp"

namespace uinta {

struct PostProcessingShaders {
  GLuint scene;
  GLuint screen;

  const file_t* f_scene_v;
  const file_t* f_scene_f;
  const file_t* f_screen_v;
  const file_t* f_screen_f;

  GLuint u_mvp, u_screen, u_texture1;

  void init(FileManager& fileManager) {
    const file_t* const sceneVert = fileManager.registerFile("scene.vert", FileType::Text);
    const file_t* const sceneFrag = fileManager.registerFile("scene.frag", FileType::Text);
    const file_t* const screenVert = fileManager.registerFile("screen.vert", FileType::Text);
    const file_t* const screenFrag = fileManager.registerFile("screen.frag", FileType::Text);
    fileManager.loadAll();

    auto sceneVertSrc = fileManager.getDataChars(sceneVert);
    auto sceneFragSrc = fileManager.getDataChars(sceneFrag);
    const std::vector<std::string> sceneSrcs({sceneVertSrc, sceneFragSrc});
    const std::vector<GLenum> sceneStages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> sceneUniforms({"mvp", "texture1"});
    const std::vector<GLuint*> sceneLocations = {&u_mvp, &u_texture1};
    scene = createShaderProgram(sceneSrcs, sceneStages, sceneUniforms, sceneLocations);

    auto screenVertSrc = fileManager.getDataChars(screenVert);
    auto screenFragSrc = fileManager.getDataChars(screenFrag);
    const std::vector<std::string> screenSrcs({screenVertSrc, screenFragSrc});
    const std::vector<GLenum> screenStages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> screenUniforms({"screenTexture"});
    const std::vector<GLuint*> screenLocations = {&u_screen};
    screen = createShaderProgram(screenSrcs, screenStages, screenUniforms, screenLocations);
  }
};

}  // namespace uinta

#endif  // UINTA_APP_POST_PROCESSING_SHADERS_HPP
