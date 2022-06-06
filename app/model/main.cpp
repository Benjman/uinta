#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <uinta/file.hpp>
#include <uinta/macros.hpp>
#include <uinta/mesh.hpp>
#include <uinta/model.hpp>
#include <uinta/shader.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../utils/utils.hpp"

#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

namespace uinta {

struct ModelRunner final : GlfwRunner {
  unsigned int icount = 0, vcount = 0;

  GLuint shader, u_model;

  ModelRunner() : GlfwRunner("hello models", 1000, 1000) {}

  void doInit() override {
    unsigned int size = KILOBYTES(100);

    GLfloat vbuf[size];
    GLuint ibuf[size];

    load_shaders();
    initBuffers();
    initObj(vbuf, size, ibuf, size);
    glEnable(GL_DEPTH_TEST);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void doPreTick(const RunnerState &state) override {
    if (state.input.isKeyPressed(KEY_SPACE)) {
      glDeleteProgram(shader);
      load_shaders();
      glUseProgram(shader);
    }
  }

  void initObj(GLfloat *const vbuf, const unsigned int vbufCount, GLuint *const ibuf, const unsigned int ibufCount) {
    const std::unordered_map<MeshAttribType, MeshAttrib> attribs = {
        {MeshAttribType_Position, MeshAttrib(6, 0)},
        {MeshAttribType_Normal,   MeshAttrib(6, 3)},
    };
    loadObj(Model_Suzanne, vbuf, &vcount, ibuf, &icount, &attribs);

    glBufferData(GL_ARRAY_BUFFER, vbufCount * sizeof(GLfloat), vbuf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibufCount * sizeof(GLuint), ibuf, GL_STATIC_DRAW);
  }

  void initBuffers() {
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  }

  void load_shaders() {
    char vert[getFileSize("shader/model.vert")];
    readFileRaw("shader/model.vert", vert);

    char frag[getFileSize("shader/model.frag")];
    readFileRaw("shader/model.frag", frag);

    const std::vector<std::string> sources({std::string(vert, sizeof(vert)), std::string(frag, sizeof(frag))});
    const std::vector<GLenum> stages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    const std::vector<std::string> uniforms({"u_model"});
    const std::vector<GLuint *> locations = {&u_model};

    shader = createShaderProgram(sources, stages, uniforms, locations);
  }

  void doRender() override {
    glm::mat4 model = glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);
    glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
  }
};

} // namespace uinta

int main(const int argc, const char **argv) { return ModelRunner().run(); }
