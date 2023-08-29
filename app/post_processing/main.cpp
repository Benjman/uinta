#include <stb/stb_image.h>
#include <uinta/gl.h>
#include <uinta/glfw/imgui.h>

#include <glm/ext/matrix_transform.hpp>
#include <uinta/error.hpp>
#include <uinta/gl/fbo.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/mesh.hpp>
#include <uinta/parsers/obj.hpp>
#include <uinta/shader.hpp>

#include "./PostProcessingShaders.hpp"

namespace uinta {

uinta_error_code initObj(const std::string& path, FileManager& fileManager, Vao& vao, Vbo& vbo, uint* icount) {
  auto cubeObj = fileManager.registerFile(path);
  fileManager.loadFile(cubeObj);
  float vertices[fileManager.getSize(cubeObj)];
  uint indices[fileManager.getSize(cubeObj)];
  uint ioff = 0;
  uint vcount = 0;
  if (auto error = loadObj(fileManager.getDataString(cubeObj), vertices, &vcount, indices, icount, &ioff,
                           {
                               {MeshAttribType_Position, {9, 0}},
                               {MeshAttribType_Normal, {9, 3}},
                               {MeshAttribType_Color, {9, 6}},
                           });
      error)
    return error;

  // cube VAO
  if (auto error = initVao(vao); error) return error;
  if (auto error = uploadVbo(vbo, vertices, vcount * sizeof(GLfloat)); error) return error;
  if (auto error = indexBuffer(vao, indices, *icount * sizeof(GLuint)); error) return error;
  if (auto error = initVertexAttribs(vao); error) return error;
  return SUCCESS_EC;
}

class PostProcessing {
 public:
  Vao quadVao = {{
      {0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0},
      {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 2 * sizeof(float)},
  }};
  Vbo quadVbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};

  uinta_error_code init() {
    // clang-format off
    float quadVertices[] = {
      // positions   // uvs
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  1.0f, 1.0f
    };
    // clang-format on
    if (auto error = initVao(quadVao); error) return error;
    if (auto error = uploadVbo(quadVbo, quadVertices, sizeof(quadVertices)); error) return error;
    if (auto error = initVertexAttribs(quadVao); error) return error;
    return SUCCESS_EC;
  }

  void render(const RunnerState& state, const Fbo& fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    bindVao(quadVao);
    glBindTexture(GL_TEXTURE_2D, fbo.renderTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
};

class PostProcessingRunner final : public GlfwRunner {
  PostProcessingShaders shaders{};
  PostProcessing pp;
  ColorShader colorShader;

  Fbo fbo;

  Vao cubeVao = {{
      {0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0},
      {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 3 * sizeof(float)},
      {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 6 * sizeof(float)},
  }};
  Vbo cubeVbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  uint icount = 0;

 public:
  PostProcessingRunner(const int argc, const char** argv)
      : GlfwRunner("Post Processing", argc, argv), fbo({window.width, window.height}) {
  }

  uinta_error_code doInit() override {
    if (auto error = GlfwRunner::doInit(); error) return error;
    if (auto error = initFbo(fbo); error) return error;
    if (auto error = pp.init(); error) return error;
    if (auto error = colorShader.init(fileManager); error) return error;
    if (auto error = shaders.init(fileManager); error) return error;
    if (auto error = initObj("model/cube.obj", fileManager, cubeVao, cubeVbo, &icount); error) return error;

    auto& cam = scene.camera;
    cam.dist = 5;
    cam.pitch = 35;
    cam.angle = 35;

    return SUCCESS_EC;
  }

  void doRender(const RunnerState& state) override {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    GlfwRunner::doRender(state);

    glUseProgram(colorShader.shaderId);
    bindVao(cubeVao);

    auto view = getViewMatrix(scene.camera);
    auto proj = getPerspectiveMatrix(scene.camera);
    auto model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(colorShader.u_mvp, 1, GL_FALSE, &(proj * view * model)[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, icount);

    glUseProgram(shaders.screen);
    glUniform1f(shaders.u_time, state.runtime);
    glUniform1f(shaders.u_scale, shaders.shimmerScale);

    pp.render(state, fbo);
    renderUi(state);
  }

  void renderUi(const RunnerState& state) {
#ifndef IMGUI_API_DISABLED
    if (ImGui::CollapsingHeader("Post Processing")) {
      float minShimmer = 0.00001;
      float maxShimmer = 0.05;
      ImGui::PushItemWidth(100);
      ImGui::DragScalar("Shimmer scale", ImGuiDataType_Float, reinterpret_cast<void*>(&shaders.shimmerScale), 0.00001,
                        &minShimmer, &maxShimmer, "%+.4f");
      ImGui::PopItemWidth();
    }
    ImGui::Separator();
#endif  // IMGUI_API_DISABLED
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::PostProcessingRunner(argc, argv).run();
}
