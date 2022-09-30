#include <stb/stb_image.h>
#include <uinta/gl.h>

#include "../utils/utils.hpp"
#include "./PostProcessingCamera.hpp"
#include "./PostProcessingShaders.hpp"

namespace uinta {

class PostProcessingRunner final : public GlfwRunner {
  PostProcessingCamera camera;
  PostProcessingShaders shaders;

  GLuint cubeTexture;
  Vao cubeVao = Vao({
      VertexAttrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0),
      VertexAttrib(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))),
  });
  Vbo cubeVbo = Vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

  Vao quadVao = Vao({
      VertexAttrib(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0),
      VertexAttrib(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))),
  });
  Vbo quadVbo = Vbo(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

  GLuint colorBufferTexture;
  GLuint fbo;
  GLuint rbo;

 public:
  PostProcessingRunner() : GlfwRunner("Hello Post Processing", 1280, 720) {
  }

  bool doInit() override {
    // shader
    const file_t* const cubeTextureFile = fileManager.registerFile("textures/container.jpg", FileType::Text);
    fileManager.loadAll();

    shaders.init(fileManager);
    glUseProgram(shaders.scene);
    glUniform1i(shaders.u_texture1, 0);

    cubeTexture = loadTexture(cubeTextureFile);

    // clang-format off
    // models
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // clang-format on

    // cube VAO
    initVao(cubeVao);
    upload(cubeVbo, cubeVertices, sizeof(cubeVertices));
    initVertexAttribs(cubeVao);

    // framebuffer configuration
    // -------------------------
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // create a color attachment texture
    glGenTextures(1, &colorBufferTexture);
    glBindTexture(GL_TEXTURE_2D, colorBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display.width, display.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTexture, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, display.width,
                          display.height);  // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);  // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      SPDLOG_ERROR("Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // screen quad
    // clang-format off
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
       1.0f, -1.0f,  1.0f, 0.0f,

      -1.0f,  1.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  1.0f, 1.0f
    };
    // clang-format on

    initVao(quadVao);
    upload(quadVbo, quadVertices, sizeof(quadVertices));
    initVertexAttribs(quadVao);

    return true;
  }

  void doTick(const RunnerState& state) override {
    camera.tick(state);
  }

  void doPreRender(const RunnerState& state) override {
    camera.updateView(state);
  }

  void doRender(const RunnerState& state) override {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);  // enable depth testing (is disabled for rendering screen-space quad)

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shaders.scene);
    bind(cubeVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    glm::mat4 view = camera.view;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), display.aspectRatio, 0.1f, 100.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(shaders.u_mvp, 1, GL_FALSE, &(projection * view * model)[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // render FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);  // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaders.screen);
    bind(quadVao);
    glBindTexture(GL_TEXTURE_2D, colorBufferTexture);  // use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  // TODO move this to some utility header
  GLuint loadTexture(const file_t* const handle) {
    int width, height, componentCount;
    unsigned char* data = stbi_load(fileManager.getPath(handle).c_str(), &width, &height, &componentCount, 0);

    if (!data) {
      SPDLOG_ERROR("Texture failed to load at path '{}'", fileManager.getPath(handle));
      return GL_ZERO;
    }

    GLuint id;
    glGenTextures(1, &id);

    GLenum format;
    if (componentCount == 1)
      format = GL_RED;
    else if (componentCount == 3)
      format = GL_RGB;
    else if (componentCount == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return id;
  }
};

}  // namespace uinta

int main(const int argc, const char** argv) {
  return uinta::PostProcessingRunner().run();
}
