#include <glm/gtc/matrix_transform.hpp>

#include "../utils/utils.hpp"

namespace uinta {

struct ModelRunner final : GlfwRunner {
  Vao vao = Vao({
      VertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0),
      VertexAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))),
  });
  Vbo vbo = Vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
  uint32_t icount = 0, vcount = 0;

  GLuint shader, u_model;
  const file_t *f_vert, *f_frag, *f_model;

  ModelRunner() : GlfwRunner("hello models", 1000, 1000) {
    f_vert = fileManager.registerFile("model.vert", FileType::Text);
    f_frag = fileManager.registerFile("model.frag", FileType::Text);
    f_model = fileManager.registerFile("model/suzanne.obj", FileType::Text);
  }

  bool doInit() override {
    glEnable(GL_DEPTH_TEST);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat vbuf[KILOBYTES(100)];
    GLuint ibuf[KILOBYTES(100)];
    initObj(vbuf, ibuf);

    initVao(vao);
    upload(vbo, vbuf, vcount * sizeof(GLfloat));
    initVertexAttribs(vao);
    indexBuffer(vao, ibuf, icount * sizeof(GLuint));

    load_shaders();

    return true;
  }

  void doPreTick(const RunnerState& state) override {
    if (isKeyPressed(state.input, KEY_SPACE)) {
      glDeleteProgram(shader);
      load_shaders();
      glUseProgram(shader);
    }
  }

  void initObj(GLfloat* const vbuf, GLuint* const ibuf) {
    const std::unordered_map<MeshAttribType, MeshAttrib> attribs = {
        {MeshAttribType_Position, MeshAttrib(6, 0)},
        {MeshAttribType_Normal, MeshAttrib(6, 3)},
    };
    uint32_t ioff = 0;
    loadObj(fileManager.getDataChars(f_model), vbuf, &vcount, ibuf, &icount, &ioff, attribs);
  }

  void load_shaders() {
    const std::vector<std::string> sources({fileManager.getDataChars(f_vert), fileManager.getDataChars(f_frag)});
    const std::vector<GLenum> stages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> uniforms({"u_model"});
    const std::vector<GLuint*> locations = {&u_model};
    shader = createShaderProgram(sources, stages, uniforms, locations);
    fileManager.releaseFile(f_vert);
    fileManager.releaseFile(f_frag);
  }

  void doRender(const RunnerState& state) override {
    clearBuffer();
    glUseProgram(shader);
    bind(vao);
    glm::mat4 model = glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
    glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::ModelRunner().run();
}
