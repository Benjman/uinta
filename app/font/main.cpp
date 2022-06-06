#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <uinta/font.hpp>
#include <uinta/macros.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../utils/utils.hpp"

#include <cstdio>

const unsigned int VBUF_SIZE = KILOBYTES(15);
const unsigned int IBUF_SIZE = KILOBYTES(15);

const unsigned int WINDOW_WIDTH  = 1000;
const unsigned int WINDOW_HEIGHT = 1000;

const input_key_t MY_KEY = KEY_SPACE;

struct FontRunner final : GlfwRunner {
public:
  font::font_t font;
  font::text text = font::text(
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna "
      "aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis "
      "aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat "
      "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
      32.0,                    // line_size
      1.0, 0.5, 0.2,           // color
      0.0, 0.0,                // pos
      WINDOW_WIDTH * 0.75, 0.0 // max dimensions
  );

  // vertex buffer
  float vbuf[VBUF_SIZE];

  // index buffer
  unsigned int ibuf[IBUF_SIZE];
  unsigned int icount = 0;

  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  GLuint shader;

  FontRunner() : GlfwRunner("hello font", WINDOW_WIDTH, WINDOW_HEIGHT) {}

  void doInit() override {
    initFont();
    initShader();
    initBuffers();
    initMesh();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  void doRender() override { glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0); }

  void initBuffers() {
    glGenVertexArrays(1, &vao);

    GLuint ids[2];
    glGenBuffers(2, ids);
    vbo = ids[0];
    ebo = ids[1];

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
  }

  void initMesh() {
    const std::unordered_map<FontMeshAttribTypes, FontMeshAttrib> attribs = {
        {FontMeshAttrib_Position, FontMeshAttrib(7, 0)},
        {FontMeshAttrib_UV,       FontMeshAttrib(7, 2)},
        {FontMeshAttrib_Color,    FontMeshAttrib(7, 4)},
    };
    unsigned int vbuf_count = 0;
    unsigned int ioffset    = 0;
    font::generate_mesh(&text, font, display.width, display.height, &attribs, vbuf, &vbuf_count, ibuf, &icount, &ioffset);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VBUF_SIZE, vbuf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * IBUF_SIZE, ibuf, GL_STATIC_DRAW);
  }

  void initShader() {
    const char *vshader = "#version 330 core\n"
                          "layout (location = 0) in vec2 in_pos;"
                          "layout (location = 1) in vec2 in_uv;"
                          "layout (location = 2) in vec3 in_color;"
                          "out vec2 pass_uv;"
                          "out vec3 pass_color;"
                          "void main() {"
                          "  pass_uv = in_uv;"
                          "  pass_color = in_color;"
                          "  gl_Position = vec4(in_pos, 0.0, 1.0);"
                          "}\0";

    const char *fshader = "#version 330 core\n"
                          "in vec2 pass_uv;"
                          "in vec3 pass_color;"
                          "uniform sampler2D atlas;"
                          "out vec4 out_color;"
                          "void main() {"
                          "  out_color = vec4(pass_color, texture(atlas, pass_uv).r);"
                          "}\0";

    const std::vector<std::string> sources({std::string(vshader, strlen(vshader)), std::string(fshader, strlen(fshader))});
    const std::vector<GLenum> stages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    shader = createShaderProgram(sources, stages);

    glUseProgram(shader);
  }

  void initFont() {
    auto type = font::DejaVuSans;
    font      = font::initFont(type, 256, 256);
    unsigned char data[getFileSize(getFontPath(type))];
    readFileBinary(getFontPath(type), (char *)data);
    load_font(font, data);
  }
};

FontRunner runner;

int main(const int argc, const char **argv) { return runner.run(); }
