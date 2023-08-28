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

  ModelManagerRunner(const int argc, const char** argv) : GlfwRunner("Model management", argc, argv) {
  }

  uinta_error_code doInit() override {
    if (auto error = GlfwRunner::doInit(); error) return error;

    // model
    const file_t* const cubeFile = fileManager.registerFile("models/cube.obj");
    fileManager.loadFile(cubeFile);
    if (auto error = modelManager.loadModel(modelId, cubeFile, fileManager); error) return error;

    auto* vtxBuffer = modelManager.getVertexBuffer(modelId);
    auto vtxSize = modelManager.getVertexBufferSize(modelId);
    auto* idxBuffer = modelManager.getIndexBuffer(modelId);
    auto idxSize = modelManager.getIndexBufferSize(modelId);

    if (auto error = initVao(vao); error) return error;
    if (auto error = uploadVbo(vbo, vtxBuffer, vtxSize); error) return error;
    if (auto error = initVertexAttribs(vao); error) return error;
    if (auto error = indexBuffer(vao, idxBuffer, idxSize); error) return error;

    // shader
    const auto vs = fileManager.registerFile("shaders/scene.vs");
    const auto fs = fileManager.registerFile("shaders/scene.fs");
    fileManager.loadFile({vs, fs});
    const std::vector<std::string> screenSrcs({fileManager.getDataString(vs), fileManager.getDataString(fs)});
    const std::vector<GLenum> screenStages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> uNames({"u_model", "u_view", "u_proj"});
    const std::vector<GLuint*> uLocations = {&u_model, &u_view, &u_proj};
    if (auto error = createShaderProgram(shader, screenSrcs, screenStages, uNames, uLocations); error) return error;

    return SUCCESS_EC;
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
