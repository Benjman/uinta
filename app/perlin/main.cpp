#include <algorithm>
#include <uinta/error.hpp>
#include <uinta/glfw/glfw_runner.hpp>
#include <uinta/input.hpp>
#include <uinta/logging.hpp>
#include <uinta/math/perlin.hpp>
#include <uinta/shader.hpp>
#include <uinta/utils/buffer2d.hpp>

namespace uinta {

class TextureShader {
 public:
  GLuint id;

  uinta_error_code init(FileManager& fileManager) {
    const auto vs = fileManager.registerFile("shader/texture.vs");
    const auto fs = fileManager.registerFile("shader/texture.fs");
    fileManager.loadFile({vs, fs});
    const std::vector<std::string> srcs({fileManager.getDataString(vs), fileManager.getDataString(fs)});
    const std::vector<GLenum> stages({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER});
    if (auto error = createShaderProgram(id, srcs, stages); error) return error;
    fileManager.releaseFile({vs, fs});
    SPDLOG_INFO("Initialized shader {}.", id);
    return SUCCESS_EC;
  }
};

class PerlinRunner : public GlfwRunner {
 public:
  TextureShader shader;
  Vao vao = {{
      {0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0},
      {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 2 * sizeof(f32)},
  }};
  Vbo vbo = {GL_ARRAY_BUFFER, GL_STATIC_DRAW};
  GLuint textureId;
  u32 perlinSeed = 1;
  f32 frequency = 1;
  u32 octaves = 1;
  glm::ivec2 offset = {0, 0};

  PerlinRunner(const i32 argc, const char** argv) : GlfwRunner("Perlin", argc, argv) {
    window(Window(window().title, 800, 800));
    clearColor = {0, 0, 0};
  }

  uinta_error_code doInit() override {
    if (auto error = GlfwRunner::doInit(); error) return error;
    if (auto error = shader.init(file_manager()); error) return error;

    // clang-format off
    const f32 vertices[] = {
        // positions    // texture coords
         1,  1,         1, 1,
         1, -1,         1, 0,
        -1, -1,         0, 0,
        -1,  1,         0, 1,
    };
    const u32 indices[] = { 0, 1, 3, 1, 2, 3 };
    // clang-format on
    vao.init(logger());
    vbo.init();
    vao.bind();
    vbo.upload(vertices, sizeof(vertices), 0);
    vao.index_buffer(indices, sizeof(indices));

    glGenTextures(1, &textureId);
    updateTexture();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SPDLOG_INFO("");
    SPDLOG_INFO("    ,========  KEYBOARD INPUTS  ========.");
    SPDLOG_INFO("    |     Action  |  Inc  |     Dec     |");
    SPDLOG_INFO("    |-----------------------------------|");
    SPDLOG_INFO("    |  Frequency  |   F   |  Shift + F  |");
    SPDLOG_INFO("    |   Offset-X  | Right |     Left    |");
    SPDLOG_INFO("    |   Offset-Y  |   Up  |     Down    |");
    SPDLOG_INFO("    |     Octave  |   O   |  Shift + O  |");
    SPDLOG_INFO("    |       Seed  |   S   |  Shift + S  |");
    SPDLOG_INFO("    `-----------------------------------'");
    SPDLOG_INFO("");

    return SUCCESS_EC;
  }

  void updateTexture() {
    const auto width = window().width;
    const auto height = window().height;
    auto textureData = Buffer2d(width, height);
    perlinNoise(textureData, siv::PerlinNoise(perlinSeed), frequency, octaves, offset);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, &textureData);
  }

  void doTick() override {
    static constexpr i32 OFFSET_DELTA = 10;
    if (isKeyPressed(input(), KEY_S)) {
      perlinSeed += isShiftDown(input()) ? -1 : 1;
      SPDLOG_INFO("Seed set to {}", perlinSeed);
      updateTexture();
    }
    if (isKeyPressed(input(), KEY_F)) {
      frequency = std::clamp(frequency + (isShiftDown(input()) ? -1 : 1), 0.0f, 64.0f);
      updateTexture();
    }
    if (isKeyPressed(input(), KEY_O)) {
      octaves = std::clamp(static_cast<i32>(octaves + (isShiftDown(input()) ? -1 : 1)), 0, 16);
      updateTexture();
    }
    if (isKeyPressed(input(), KEY_LEFT)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input()) ? 10 : 1);
      offset += glm::ivec2(-delta, 0);
      updateTexture();
    }
    if (isKeyPressed(input(), KEY_RIGHT)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input()) ? 10 : 1);
      offset += glm::ivec2(delta, 0);
      updateTexture();
    }
    if (isKeyPressed(input(), KEY_UP)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input()) ? 10 : 1);
      offset += glm::ivec2(0, delta);
      updateTexture();
    }
    if (isKeyPressed(input(), KEY_DOWN)) {
      const auto delta = OFFSET_DELTA * (isShiftDown(input()) ? 10 : 1);
      offset += glm::ivec2(0, -delta);
      updateTexture();
    }
  }

  void doPreRender() override {
  }

  void doPostRender() override {
  }

  void doRender() override {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUseProgram(shader.id);
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::PerlinRunner(argc, argv).run();
}
