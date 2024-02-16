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
    : appConfig_(params.appConfig),
      fileSystem_(params.fileSystem),
      gl_(params.gl),
      platform_(params.platform),
      shader_({{GL_VERTEX_SHADER, "shader.vs.glsl"},
               {GL_FRAGMENT_SHADER, "shader.fs.glsl"}},
              params.gl, params.fileSystem),
      uProjection_("uProjection", &shader_),
      uColor_("uColor", &shader_),
      vao_(params.gl),
      vbo_(GL_ARRAY_BUFFER, 0, params.gl) {
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

  constexpr f32 fov = 45;
  constexpr f32 nearPlane = 0.1;
  constexpr f32 farPlane = 5;
  dispatchers_.addListener<EngineEvent::ViewportSizeChange>(
      [&](const auto& event) {
        ShaderGuard guard(&shader_);
        uProjection_ =
            glm::perspective(fov, event.aspect(), nearPlane, farPlane);
      });

  constexpr f32 triLen = (0.2 * 2) / glm::root_three<f32>();
  constexpr f32 rectHeight = 0.2f;
  constexpr f32 rectWidth = (16.0 / 9.0) * rectHeight;
  std::array<f32, 30> vertices = {
      0.0f, 0.2 / 2.0f,             // Tri Vertex 1 - top
      -triLen / 2.0f, -0.2 / 2.0f,  // Tri Vertex 2 - bottom left
      triLen / 2.0f, -0.2 / 2.0f,   // Tri Vertex 3 - bottom right

      // 16:9 rectangle
      -rectWidth / 2.0f, rectHeight / 2.0f,   // Rect vertex top-left
      rectWidth / 2.0f, rectHeight / 2.0f,    // Rect vertex top-right
      -rectWidth / 2.0f, -rectHeight / 2.0f,  // Rect vertex bottom-left
      -rectWidth / 2.0f, -rectHeight / 2.0f,  // Rect vertex bottom-left
      rectWidth / 2.0f, -rectHeight / 2.0f,   // Rect vertex bottom-right
      rectWidth / 2.0f, rectHeight / 2.0f,    // Rect vertex top-right
  };

  std::copy(vertices.begin() + 6, vertices.begin() + 18, vertices.begin() + 18);

  for (std::size_t i = 0; i < 6; i += 2) vertices.at(i) -= 0.5;
  for (std::size_t i = 18; i < vertices.size(); i += 2) vertices.at(i) += 0.5;
  for (std::size_t i = 6; i < 18; i += 2) {
    constexpr f32 separation = 0.005;
    vertices.at(i) += (i >= 12 ? 1 : -1) * separation;
  }

  {
    VboGuard vbg(&vbo_);
    VaoGuard vag(&vao_);
    vbo_.bufferData(vertices.data(), vertices.size() * sizeof(GLfloat),
                    GL_STATIC_DRAW);
    vao_.linkAttribute({0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0});
  }
}

void Engine::run() noexcept {
  while (!state_.isClosing() && status_.ok()) {
    if (auto status = platform_->pollEvents(); !status.ok()) {
      setStatusError(status);
      break;
    }

    state_.updateRuntime(getRuntime());

    advance<EngineStage::PreTick>();
    advance<EngineStage::Tick>();
    advance<EngineStage::PostTick>();

    state_.addTick();

    dispatchers_.dispatch<EngineEvent::TickComplete>(
        TickComplete(&state_, getRuntime()));

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
  uColor_ = palette[1];
  gl_->drawArrays(GL_TRIANGLES, 0, 3);  // Triangle
  uColor_ = palette[2];
  gl_->drawArrays(GL_TRIANGLES, 3, 3);  // Top separated rectangle
  uColor_ = palette[3];
  gl_->drawArrays(GL_TRIANGLES, 6, 3);  // Bottom separated rectangle
  uColor_ = palette[4];
  gl_->drawArrays(GL_TRIANGLES, 9, 6);  // Rectangle
}

void Engine::postRender() noexcept {}

}  // namespace uinta
