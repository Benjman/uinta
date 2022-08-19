// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cstdlib>
#include <cstring>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

#include "../utils/utils.hpp"

namespace uinta {

struct PostProcessingRunner final : GlfwRunner {
  uint32_t indexCount = 0;

  GLuint shader, u_mvp, u_near, u_far;
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  StaticCamera camera;
  float nearPlane = 0.1;
  float farPlane = 15.0;

  PostProcessingRunner() : GlfwRunner("hello fbo", 1000, 1000) {
    camera.position = glm::vec3(0, 0, 5);
    camera.pitch = 0.0;
  }

  const file_t *vert_file, *frag_file, *cube_file;

  void doInitFiles() override {
    vert_file = fileManager.registerFile("post_processing.vert", FileType::Text);
    frag_file = fileManager.registerFile("post_processing.frag", FileType::Text);
    cube_file = fileManager.registerFile(getObjPath(Model_Cube), FileType::Text);
  }

  bool doInit() override {
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    initShaders();
    initBuffers();
    initObj();
    initScene();
    initView();

    glEnable(GL_DEPTH_TEST);

    return true;
  }

  void doPreTick(const RunnerState& state) override {
    if (state.input.isKeyPressed(KEY_SPACE)) {
      glDeleteProgram(shader);
      initShaders();
    }
  }

  void initObj() {
    uint32_t size = KILOBYTES(100);
    uint32_t vertexCount = 0;
    GLfloat vbuf[size];
    GLuint ibuf[size];

    const std::unordered_map<MeshAttribType, MeshAttrib> attribs = {
        {MeshAttribType_Position, MeshAttrib(3, 0)},
    };

    loadObj(fileManager.getDataChars(cube_file), vbuf, &vertexCount, ibuf, &indexCount, &attribs);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vbuf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), ibuf, GL_STATIC_DRAW);
  }

  void initBuffers() {
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
  }

  void initScene() {}

  void initView() {
    float fov = glm::radians(45.0);
    projection = glm::perspective(fov, display.aspectRatio, nearPlane, farPlane);
    genViewMatrix(view, camera.position, camera.pitch, camera.yaw);
  }

  void initShaders() {
    const std::vector<std::string> sources({fileManager.getDataChars(vert_file), fileManager.getDataChars(frag_file)});
    const std::vector<GLenum> stages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> uniforms({"u_mvp", "u_nearPlane", "u_farPlane"});
    const std::vector<GLuint*> locations = {&u_mvp, &u_near, &u_far};

    shader = createShaderProgram(sources, stages, uniforms, locations);
    // fileManager.releaseFile(vert_file);
    // fileManager.releaseFile(frag_file);

    glUseProgram(shader);

    glUniform1f(u_near, nearPlane);
    glUniform1f(u_far, farPlane);
  }

  void doRender() override {
    model = glm::mat4(1.0);
    // model = glm::scale(model, glm::vec3(0.5));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &mvp[0][0]);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) { return uinta::PostProcessingRunner().run(); }
