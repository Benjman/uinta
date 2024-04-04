#include "uinta/engine/engine.h"

#include <cassert>
#include <string>

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "glm/ext/vector_float4.hpp"

namespace uinta {

constexpr std::array<glm::vec4, 5> theGrassIsAlwaysGreener = {
    glm::vec4(58.0 / 255.0, 57.0 / 255.0, 66.0 / 255.0, 1.0),
    glm::vec4(255.0 / 255.0, 233.0 / 255.0, 189.0 / 255.0, 1.0),
    glm::vec4(209.0 / 255.0, 202.0 / 255.0, 147.0 / 255.0, 1.0),
    glm::vec4(177.0 / 255.0, 188.0 / 255.0, 122.0 / 255.0, 1.0),
    glm::vec4(130.0 / 255.0, 168.0 / 255.0, 93.0 / 255.0, 1.0),
};

constexpr auto palette = theGrassIsAlwaysGreener;

Engine::Engine(Params params) noexcept
    : frame_(params.platform->primaryMonitor().value_or(nullptr)),
      appConfig_(params.appConfig),
      fileSystem_(params.fileSystem),
      gl_(params.gl),
      platform_(params.platform),
      shader_({{GL_VERTEX_SHADER, "shader.vs.glsl"},
               {GL_FRAGMENT_SHADER, "shader.fs.glsl"}},
              params.gl, params.fileSystem),
      uProjection_("uProjection", &shader_),
      uColor_("uColor", &shader_),
      vao_(params.gl),
      vbo_(GL_ARRAY_BUFFER, 0, params.gl),
      texture_(GL_TEXTURE_2D, 0, 0, 0, 0, 0, params.gl) {
  assert(platform_ && "`Platform*` cannot be null.");

  platform_->engine(this);

  platform_->addListener<PlatformEvent::OnCloseRequest>(
      [this](const auto&) { state_.isClosing(true); });

  platform_->addListener<PlatformEvent::OnError>([this](const auto& event) {
    auto msg = absl::StrFormat("%i: %s", event.code, event.description);
    setStatusError(InternalError(msg));
  });

  platform_->addListener<PlatformEvent::OnDebugMessage>([](const auto& event) {
    std::string message(event.message, event.length);
    message = absl::StrCat(message, "\n\tID: ", event.id);
    message = absl::StrCat(message, "\n\tSeverity: ",
                           OpenGLApi::GetSeverityString(event.severity));
    message = absl::StrCat(message, "\n\tSource: ", event.source, "\t",
                           OpenGLApi::GetSourceString(event.source));
    message = absl::StrCat(message, "\n\tType: ", event.type, "\t",
                           OpenGLApi::GetTypeString(event.type));

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

  platform_->addListener<PlatformEvent::OnViewportSizeChange>(
      [this](const auto&) {
        auto width = platform_->window()->width();
        auto height = platform_->window()->height();
        gl_->viewport(0, 0, width, height);
        dispatchers_.dispatch<EngineEvent::ViewportSizeChange>(
            ViewportSizeChange(width, height));
        LOG(INFO) << absl::StrFormat("Event: Viewport size change (%u, %u)",
                                     width, height);
      });

  gl_->clearColor(0.1, 0.1, 0.1, 1.0);

  platform_->addListener<PlatformEvent::OnMonitorChange>(
      [this](const auto& event) { frame_ = FrameManager(event.monitor); });

  constexpr f32 fov = 45;
  constexpr f32 nearPlane = 0.1;
  constexpr f32 farPlane = 5;
  dispatchers_.addListener<EngineEvent::ViewportSizeChange>(
      [&](const auto& event) {
        ShaderGuard guard(&shader_);
        uProjection_ =
            glm::perspective(fov, event.aspect(), nearPlane, farPlane);
      });

  std::array<f32, 16> vertices = {
      -0.32f, 0.45f,  0.0f, 1.0f,  // top-left
      0.32f,  0.45f,  1.0f, 1.0f,  // top-right
      -0.32f, -0.45f, 0.0f, 0.0f,  // bottom-left
      0.32f,  -0.45f, 1.0f, 0.0f,  // bottom-right
  };
  {
    VboGuard vbg(&vbo_);
    VaoGuard vag(&vao_);
    vbo_.bufferData(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
    vao_.linkAttribute({0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0});
    vao_.linkAttribute(
        {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat)});
  }
  if (status_ = texture_.fromFile("texture.jpg", fileSystem_); !status_.ok()) {
    return;
  }
}

void Engine::run() noexcept {
  while (!state_.isClosing() && status_.ok()) {
    if (auto status = platform_->pollEvents(); !status.ok()) {
      setStatusError(status);
      break;
    }

    do {
      state_.updateRuntime(getRuntime());

      advance<EngineStage::PreTick>();
      advance<EngineStage::Tick>();
      advance<EngineStage::PostTick>();

      state_.addTick();

      dispatchers_.dispatch<EngineEvent::TickComplete>(
          TickComplete(&state_, getRuntime()));
    } while (state_.runtime() < frame_.next);

    state_.updateRuntime(getRuntime());

    gl_->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();

    if (auto status = platform_->swapBuffers(); !status.ok()) {
      setStatusError(status);
      break;
    }

    state_.addFrame();

    frame_.next = state_.runtime();
    if (!state_.isFixedTickRate()) {
      frame_.next += frame_.frequency;
    }

    dispatchers_.dispatch<EngineEvent::RenderComplete>(
        RenderComplete(&state_, getRuntime()));
  }
}

void Engine::preTick() noexcept {}

void Engine::tick() noexcept {}

void Engine::postTick() noexcept {}

void Engine::preRender() noexcept {}

void Engine::render() noexcept {
  ShaderGuard shaderGuard(&shader_);
  VaoGuard vaoGuard(&vao_);
  TextureGuard textureGuard(&texture_);
  gl_->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Engine::postRender() noexcept {}

}  // namespace uinta
