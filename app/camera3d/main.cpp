#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../utils/utils.hpp"

namespace uinta {

struct Camera3dRunner final : GlfwRunner {
  Camera camera;

  Vao vao{{{0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0},
           {1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 3 * sizeof(GLfloat)},
           {2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 6 * sizeof(GLfloat)}}};
  Vbo vbo{GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  uint32_t icount = 0, vcount = 0;

  GLuint u_mvp;
  glm::mat4 model;

  GLuint shader;

  const file_t *vert, *frag, *cube;

  Camera3dRunner() : GlfwRunner("hello camera3d", 1000, 1000) {
    model = glm::mat4(1.0);
    vert = fileManager.registerFile("camera3d.vert", FileType::Text);
    frag = fileManager.registerFile("camera3d.frag", FileType::Text);
    cube = fileManager.registerFile("model/cube.obj", FileType::Text);
  }

  bool doInit() override {
    initShader();

    GLfloat vertices[KILOBYTES(1)];
    GLuint indices[KILOBYTES(1)];
    initGround(vertices, indices);

    initVao(vao);
    upload(vbo, vertices, vcount * sizeof(GLfloat));
    initVertexAttribs(vao);
    indexBuffer(vao, indices, icount * sizeof(GLuint));

    glEnable(GL_DEPTH_TEST);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
  }

  void initGround(GLfloat* const vertices, GLuint* const indices) {
    uint32_t local_vcount = 0, local_icount = 0, ioff = 0;
    const MeshAttrib pos_attrib(9, 0), norm_attrib(9, 3), color_attrib(9, 6);
    const std::unordered_map<MeshAttribType, MeshAttrib> attribs = {
        {MeshAttribType_Position, pos_attrib},
        {MeshAttribType_Normal, norm_attrib},
        {MeshAttribType_Color, color_attrib},
    };
    loadObj(fileManager.getDataChars(cube), vertices, &local_vcount, indices, &local_icount, &ioff, attribs);

    glm::vec3 top(0.051, 0.933, 0.996);
    glm::vec3 sides(0.025, 0.465, 0.465);
    glm::mat4 transform = glm::scale(glm::mat4(1.0), {15, 3, 15});

    local_vcount *= 1.5;  // loadObj doesn't load colors, so we adjust for color attrib
    for (int i = 0; i < local_vcount; i += pos_attrib.stride) {
      // transform to floor
      auto pos = transform * glm::vec4(vertices[i + pos_attrib.offset + 0], vertices[i + pos_attrib.offset + 1] - 1,
                                       vertices[i + pos_attrib.offset + 2], 1.0);
      memcpy(&vertices[i + pos_attrib.offset], &pos[0], 3 * sizeof(GLfloat));

      // colorize
      glm::vec3 color(0);
      glm::vec3 norm = glm::vec3(vertices[i + norm_attrib.offset + 0], vertices[i + norm_attrib.offset + 1],
                                 vertices[i + norm_attrib.offset + 2]);
      color += sides * glm::abs(glm::dot(norm, glm::vec3(1, 0, 1)));      // paint sides
      color += top * std::max(0.0f, glm::dot(norm, glm::vec3(0, 1, 0)));  // paint top
      memcpy(&vertices[i + color_attrib.offset], &color[0], 3 * sizeof(GLfloat));
    }

    vcount += local_vcount;
    icount += local_icount;

    fileManager.releaseFile(cube);
  }

  void initShader() {
    const std::vector<std::string> sources = {fileManager.getDataChars(vert), fileManager.getDataChars(frag)};
    shader = createShaderProgram(sources, {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {"u_mvp"}, {&u_mvp});
    fileManager.releaseFile(vert);
    fileManager.releaseFile(frag);
  }

  void doTick(const RunnerState& state) override {
    update(camera, state);
  }

  void doPreRender(const RunnerState& state) override {
    glUseProgram(shader);
    bind(vao);

    auto transform_x = glm::eulerAngleX(glm::radians(camera.pitch));
    auto transform_y = glm::eulerAngleY(glm::radians(camera.yaw));
    auto view = glm::translate(transform_x * transform_y, -camera.position);

    auto projection =
        glm::perspective(glm::radians(camera.config.fov), display.aspectRatio, camera.config.nearPlane, camera.config.farPlane);

    glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &(projection * view * model)[0][0]);

    clearBuffer();
  }

  void doRender(const RunnerState& state) override {
    glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
    imgui::view::camera(camera);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::Camera3dRunner().run();
}
