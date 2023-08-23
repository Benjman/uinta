#include <uinta/glfw/glfw_runner.hpp>

// clang-format off
#include <uinta/model_manager.hpp>
#include <uinta/gl/fwd.hpp>
#include <uinta/shader.hpp>
#include <glm/gtc/type_ptr.hpp>
// clang-format on

namespace uinta {

class ModelManagerRunner : public GlfwRunner {
 public:
  Vao vao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 3 * sizeof(float)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 6 * sizeof(float)},
  }};
  Vbo vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  GLuint shader, u_model, u_view, u_proj;
  ModelManager modelManager;
  model_t modelId;

  ModelManagerRunner(const int argc, const char** argv) : GlfwRunner("Model Manager Example", 1920, 1080, argc, argv) {
  }

  bool doInit() override {
    if (!GlfwRunner::doInit()) return false;

    // model
    const file_t* const cubeFile = fileManager.registerFile("models/cube.obj");
    fileManager.loadFile(cubeFile);
    modelId = modelManager.loadModel(cubeFile, &fileManager);

    initVao(vao);
    uploadVbo(vbo, modelManager.getVertexBuffer(modelId), modelManager.getVertexBufferSize(modelId));
    initVertexAttribs(vao);
    indexBuffer(vao, modelManager.getIndexBuffer(modelId), modelManager.getIndexBufferSize(modelId));

    // shader
    auto vs = fileManager.registerFile("shaders/scene.vs");
    auto fs = fileManager.registerFile("shaders/scene.fs");
    fileManager.loadFile({vs, fs});
    const std::vector<std::string> screenSrcs({fileManager.getDataString(vs), fileManager.getDataString(fs)});
    const std::vector<GLenum> screenStages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> uNames({"u_model", "u_view", "u_proj"});
    const std::vector<GLuint*> uLocations = {&u_model, &u_view, &u_proj};
    if ((shader = createShaderProgram(screenSrcs, screenStages, uNames, uLocations)) == GL_ZERO) return false;

    return shader != GL_ZERO;
  }

  void doRender(const RunnerState& state) override {
    GlfwRunner::doRender(state);

    glUseProgram(shader);
    bindVao(vao);

    glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(getViewMatrix(scene.camera)));
    glUniformMatrix4fv(u_proj, 1, GL_FALSE, glm::value_ptr(getPerspectiveMatrix(scene.camera)));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::ModelManagerRunner(argc, argv).run();
}
