#include "./cartesian_guides.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <uinta/io/file_manager.hpp>
#include <uinta/runner/display.hpp>
#include <uinta/shader.hpp>

using namespace uinta;

void CartesianGuides::init(FileManager& fm, const Display& display) {
  auto f_vert = fm.registerFile("shader/cartesianGuides.vert", FileType::Text);
  auto f_frag = fm.registerFile("shader/cartesianGuides.frag", FileType::Text);
  fm.loadFiles({f_vert, f_frag});

  std::vector<std::string> sources = {fm.getDataChars(f_vert), fm.getDataChars(f_frag)};
  std::vector<GLenum> stages = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  std::vector<std::string> uniforms = {"u_mvp", "u_color", "u_viewport", "u_bezierStrength"};
  std::vector<GLuint*> locations = {&u_mvp, &u_color, &u_viewport, &u_bezierStrength};
  shader = createShaderProgram(sources, stages, uniforms, locations);
  fm.releaseFiles({f_vert, f_frag});

  glUseProgram(shader);
  glUniform2fv(u_viewport, 1, glm::value_ptr(glm::vec2(display.width, display.height)));
  glUniform1f(u_bezierStrength, 1.0);

  // clang-format off
  float cube[] = {
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,

     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,

    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
  };
  // clang-format on

  initVao(vao);
  upload(vbo, cube, sizeof(cube));
  initVertexAttribs(vao);
}

void CartesianGuides::doRender(const RunnerState& state, const glm::mat4& projView) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(shader);
  bind(vao);

  float lineSize = 0.03;
  glm::mat4 model;

  // x
  model = glm::scale(glm::mat4(1.0), {200, lineSize, lineSize});
  glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(projView * model));
  glUniform3fv(u_color, 1, glm::value_ptr(glm::vec3(1, 0, 0)));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // y
  model = glm::scale(glm::mat4(1.0), {lineSize, 200, lineSize});
  glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(projView * model));
  glUniform3fv(u_color, 1, glm::value_ptr(glm::vec3(0, 1, 0)));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // z
  model = glm::scale(glm::mat4(1.0), {lineSize, lineSize, 200});
  glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(projView * model));
  glUniform3fv(u_color, 1, glm::value_ptr(glm::vec3(0, 0, 1)));
  glDrawArrays(GL_TRIANGLES, 0, 36);
}
