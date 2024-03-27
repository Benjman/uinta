#include "uinta/engine.h"

#include <cassert>

#include "absl/log/log.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/platform.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

Engine::Engine(Platform* platform, const OpenGLApi* gl) noexcept
    : gl_(gl), platform_(platform) {
  assert(platform_ && "`Platform*` cannot be null.");

  setCallbacks();

  Shader shader({{GL_VERTEX_SHADER, "shader.vs.glsl"},
                 {GL_FRAGMENT_SHADER, "shader.fs.glsl"}});

  Vbo vbo(GL_ARRAY_BUFFER);
  std::vector<f32> vertices = {
      // positions  // uv coords
      0.5f,  0.5f,  1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f   // top left
  };
  VboGuard vbg(&vbo);
  vbo.bufferData(vertices.data(), vertices.size() * sizeof(f32),
                 GL_STATIC_DRAW);

  Vao vao;
  std::vector<u32> idxBuffer = {0, 1, 3, 1, 2, 3};
  VaoGuard vag(&vao);
  vao.ebo(idxBuffer);
  vao.linkAttribute({0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0});
  vao.linkAttribute(
      {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat)});

  Texture texture;
  if (status_ = texture.fromFile("wall.jpg"); !status_.ok()) {
    return;
  }

  onViewportChange(platform_->window()->width(), platform_->window()->height());

  while (!state_.isClosing() && status_.ok()) {
    if (auto status = platform_->runtime(); status.ok()) {
      state_.addTicks(1);
      state_.addDelta(status.value() - state_.runtime());
    } else {
      status_ = status.status();
      return;
    }

    status_ = platform_->pollEvents();
    if (!status_.ok()) return;

    newFrame();

    ShaderGuard shaderGuard(&shader);
    VaoGuard vaoGuard(&vao);
    TextureGuard textureGuard(&texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
}

void Engine::newFrame() noexcept {
  status_ = platform_->swapBuffers();
  if (!status_.ok()) return;

  status_ = platform_->clearFrontBuffer(
      0.1, 0.1, 0.1, 1.0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!status_.ok()) return;

  state_.newFrame(true);
  state_.resetFrameDelta();
}

void Engine::setCallbacks() noexcept {
  platform_->onCloseRequest([&](void*) { state_.setClosing(); });

  platform_->onError([&](i32 code, const char* description) {
    status_ = InternalError(StrFormat("%i: %s", code, description));
  });

  platform_->onDebugMessage([&](GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const char* message) {
    auto fmtmsg = StrFormat(
        "%s\n\tID: %i\n\tSeverity: %s\n\tSource: %i\t%s\n\tType:   %i\t%s",
        std::string(message, length), id, gl_->getSeverityString(severity),
        source, gl_->getSourceString(source), type, gl_->getTypeString(type));
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
      LOG(INFO) << fmtmsg;
    else if (severity == GL_DEBUG_SEVERITY_LOW)
      LOG(WARNING) << fmtmsg;
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
      LOG(ERROR) << fmtmsg;
    else
      LOG(FATAL) << fmtmsg;
  });

  platform_->onFramebufferSize(
      [&](u32 width, u32 height) { onViewportChange(width, height); });
}

void Engine::onViewportChange(u32 width, u32 height) noexcept {
  gl_->viewport(0, 0, width, height);
}

}  // namespace uinta
