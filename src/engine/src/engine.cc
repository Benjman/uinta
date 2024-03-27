#include "uinta/engine/engine.h"

#include <cassert>

#include "absl/log/log.h"
#include "uinta/gl.h"
#include "uinta/lib/absl/strings.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

Engine::Engine(Platform* platform, const OpenGLApi* gl) noexcept
    : gl_(gl), platform_(platform) {
  assert(platform_ && "`Platform*` cannot be null.");

  platform_->addListener<PlatformEvent::OnCloseRequest>(
      [&](const auto&) { state_.isClosing(true); });

  platform_->addListener<PlatformEvent::OnError>([&](const auto& event) {
    status_ = InternalError(StrFormat("%i: %s", event.code, event.description));
  });

  platform_->addListener<PlatformEvent::OnDebugMessage>([&](const auto& event) {
    std::string message(event.message, event.length);
    message = StrCat(message, "\n\tID: ", event.id);
    message = StrCat(message,
                     "\n\tSeverity: ", gl_->getSeverityString(event.severity));
    message = StrCat(message, "\n\tSource: ", event.source, "\t",
                     gl_->getSourceString(event.source));
    message = StrCat(message, "\n\tType: ", event.type, "\t",
                     gl_->getTypeString(event.type));

    switch (event.severity) {
      case GL_DEBUG_SEVERITY_NOTIFICATION:
        LOG(INFO) << message;
        break;
      case GL_DEBUG_SEVERITY_LOW:
        LOG(WARNING) << message;
        break;
      case GL_DEBUG_SEVERITY_MEDIUM:
        LOG(ERROR) << message;
        break;
      default:
        LOG(FATAL) << message;
        break;
    }
  });

  platform_->addListener<PlatformEvent::OnViewportSizeChange>([&](const auto&) {
    auto width = platform_->window()->width();
    auto height = platform_->window()->height();
    gl_->viewport(0, 0, width, height);
    dispatch<EngineEvent::ViewportSizeChange>(
        ViewportSizeChange(width, height));
    LOG(INFO) << StrFormat("Event: Viewport size change (%u, %u)", width,
                           height);
  });

  gl_->clearColor(0.1, 0.1, 0.1, 1.0);

  state_.update(getRuntime(), 0);
}

void Engine::run() noexcept {
  state_.update(getRuntime(), 0);

  time_t start;
  time_t runtime;

  Shader shader({{GL_VERTEX_SHADER, "shader.vs.glsl"},
                 {GL_FRAGMENT_SHADER, "shader.fs.glsl"}},
                gl_);

  UniformMatrix4fv projection("uProjection", &shader);

  addListener<EngineEvent::ViewportSizeChange>([&](const auto& event) {
    ShaderGuard guard(&shader);
    projection = glm::perspective<f32>(45, event.aspect(), 0.1, 2);
  });

  Vbo vbo(GL_ARRAY_BUFFER, 0, gl_);
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

  Vao vao(gl_);
  std::vector<u32> idxBuffer = {0, 1, 3, 1, 2, 3};
  VaoGuard vag(&vao);
  vao.ebo(idxBuffer);
  vao.linkAttribute({0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0});
  vao.linkAttribute(
      {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat)});

  Texture texture(GL_TEXTURE_2D, 0, 0, 0, 0, 0, gl_);
  if (status_ = texture.fromFile("wall.jpg"); !status_.ok()) {
    return;
  }

  while (!state_.isClosing() && status_.ok()) {
    if (status_ = platform_->pollEvents(); !status_.ok()) {
      LOG(ERROR) << "`Platform::pollEvents()` failed: " << status_.message();
      break;
    }

    do {
      runtime = getRuntime();
      start = runtime;
      state_.isNewFrame(true);
      state_.update(runtime, 1);
      advance<EngineStage::PreTick>();
      advance<EngineStage::Tick>();
      advance<EngineStage::PostTick>();
      runtime = getRuntime();
      dispatch<EngineEvent::TickComplete>(
          TickComplete(&state_, runtime - start));
    } while (!state_.isNewFrame());

    runtime = getRuntime();
    start = runtime;

    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();

    ShaderGuard shaderGuard(&shader);
    VaoGuard vaoGuard(&vao);
    TextureGuard textureGuard(&texture);
    gl_->drawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    state_.isNewFrame(false);
    runtime = getRuntime();
    dispatch<EngineEvent::RenderComplete>(
        RenderComplete(&state_, runtime - start));

    if (status_ = platform_->swapBuffers(); !status_.ok()) {
      LOG(ERROR) << "`Platform::swapBuffers()` failed: " << status_.message();
      return;
    }

    gl_->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

void Engine::preTick() noexcept {}

void Engine::tick() noexcept {}

void Engine::postTick() noexcept {}

void Engine::preRender() noexcept {}

void Engine::render() noexcept {}

void Engine::postRender() noexcept {}

}  // namespace uinta
