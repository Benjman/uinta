#include <glm/gtc/type_ptr.hpp>

#include "../utils/utils.hpp"

namespace uinta {

struct ModelRunner final : GlfwRunner {
  TargetCamera cam;
  CartesianGrid grid;

  Vao vao{{
      {0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))},
  }};
  Vbo vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  uint icount = 0;

  uint shader;
  uint u_model;
  uint u_mvp;

  ModelRunner() : GlfwRunner("hello models", 1920, 1080) {
    force(cam.angle, 315);
    force(cam.pitch, 45);
    force(cam.dist, 10);
  }

  bool doInit() override {
    glEnable(GL_DEPTH_TEST);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.init(fileManager);
    initObj();
    initShader();

    return true;
  }

  void doTick(const RunnerState& state) override {
    update(cam, state);
  }

  void initObj() {
    auto file = fileManager.registerFile("model/test.obj", FileType::Text);
    fileManager.loadFile(file);
    GLfloat vbuf[KILOBYTES(100)];
    GLuint ibuf[KILOBYTES(100)];
    uint ioff = 0;
    uint vcount = 0;
    loadObj(fileManager.getDataString(file), vbuf, &vcount, ibuf, &icount, &ioff,
            {
                {MeshAttribType_Position, {6, 0}},
                {MeshAttribType_Normal, {6, 3}},
            });
    initVao(vao);
    upload(vbo, vbuf, vcount * sizeof(GLfloat));
    initVertexAttribs(vao);
    indexBuffer(vao, ibuf, icount * sizeof(GLuint));
  }

  void initShader() {
    auto vert = fileManager.registerFile("model.vs");
    auto frag = fileManager.registerFile("model.fs");
    fileManager.loadFile({vert, frag});
    const std::vector<std::string> sources({fileManager.getDataString(vert), fileManager.getDataString(frag)});
    shader = createShaderProgram(sources, {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"u_model", "u_mvp"}, {&u_model, &u_mvp});
    fileManager.releaseFile({vert, frag});
  }

  void doRender(const RunnerState& state) override {
    clearBuffer();
    glUseProgram(shader);
    bind(vao);

    auto proj = glm::perspective(glm::radians(45.f), display.aspectRatio, 0.1f, 100.0f);

    glm::mat4 model(1);
    // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(90.f), {1, 0, 0});
    model = glm::scale(model, glm::vec3(0.5));
    glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(proj * getViewMatrix(cam) * model));
    glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);

    grid.render(proj * getViewMatrix(cam));

    imgui::view::camera(cam);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::ModelRunner().run();
}
