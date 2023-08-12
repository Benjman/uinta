#include <stb/stb_image.h>
#include <uinta/gl.h>
#include <uinta/glfw/imgui.h>

#include <glm/ext/matrix_transform.hpp>
#include <uinta/gl/fbo.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/mesh.hpp>
#include <uinta/parsers/obj.hpp>
#include <uinta/shader.hpp>

#include "./PostProcessingShaders.hpp"

namespace uinta {

void initObj(const std::string& path, FileManager& fileManager, Vao& vao, Vbo& vbo, uint* icount) {
  auto cubeObj = fileManager.registerFile(path);
  fileManager.loadFile(cubeObj);
  float vertices[fileManager.getSize(cubeObj)];
  uint indices[fileManager.getSize(cubeObj)];
  uint ioff = 0;
  uint vcount = 0;
  loadObj(fileManager.getDataString(cubeObj), vertices, &vcount, indices, icount, &ioff,
          {
              {MeshAttribType_Position, {9, 0}},
              {MeshAttribType_Normal, {9, 3}},
              {MeshAttribType_Color, {9, 6}},
          });

  // cube VAO
  initVao(vao);
  uploadVbo(vbo, vertices, vcount * sizeof(GLfloat));
  indexBuffer(vao, indices, *icount * sizeof(GLuint));
  initVertexAttribs(vao);
}

class PostProcessing {
 public:
  Vao quadVao = {{
      {0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0},
      {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 2 * sizeof(float)},
  }};
  Vbo quadVbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};

  void init() {
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
    initVao(quadVao);
    uploadVbo(quadVbo, quadVertices, sizeof(quadVertices));
    initVertexAttribs(quadVao);
  }

  void render(const RunnerState& state, const Fbo& fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
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
      : GlfwRunner("Post Processing", argc, argv), fbo({display.width, display.height}) {
  }

  bool doInit() override {
    if (!GlfwRunner::doInit()) return false;

    initFbo(fbo);
    pp.init();
    colorShader.init(fileManager);

    shaders.init(fileManager);
    initObj("model/cube.obj", fileManager, cubeVao, cubeVbo, &icount);
    return true;
  }

  void doRender(const RunnerState& state) override {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    GlfwRunner::doRender(state);

    glUseProgram(colorShader.shaderId);
    bindVao(cubeVao);

    auto view = getViewMatrix(camera);
    auto proj = getPerspectiveMatrix(camera, display);
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
