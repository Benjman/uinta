#include "uinta/engine/engine.h"

#include <absl/log/log.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>

#include <cassert>
#include <memory>
#include <string>

#include "uinta/app_config.h"
#include "uinta/args.h"
#include "uinta/gl.h"
#include "uinta/localization/locale.h"
#include "uinta/localization/localization_system.h"
#include "uinta/shader.h"
#include "uinta/texture.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

namespace {

std::unique_ptr<Shader> shader;
std::unique_ptr<UniformMatrix4fv> uProjection;
std::unique_ptr<Uniform4fv> uColor;
std::unique_ptr<Vao> vao;
std::unique_ptr<Vbo> vbo;
std::unique_ptr<Texture> texture;

Locale resolveLocale(const ArgsProcessor* args) noexcept {
  assert(args && "Engine::Engine(): ArgsProcessor cannot be null!");
  if (auto val = args->getValue(ArgsProcessor::Locale)) {
    return toLocale(*val);
  }
  return Locale::EnUs;
}

time_t frameInterval(const Monitor* monitor) noexcept {
  if (monitor == nullptr || monitor->hz() == 0) {
    LOG(WARNING) << "Monitor refresh rate unavailable, defaulting frame interval to 60hz";
    return 1.0 / 60;
  }
  return 1.0 / monitor->hz();
}

}  // namespace

Engine::Engine(const EngineDependencies& deps) noexcept
    : platform_(deps.platform), localization_(resolveLocale(deps.args)) {
  assert(deps.appConfig && "Engine::Engine(): AppConfig cannot be null!");
  registerService<AppConfig>(deps.appConfig);
  assert(deps.args && "Engine::Engine(): ArgsProcessor cannot be null!");
  registerService<const ArgsProcessor>(deps.args);
  assert(deps.gl && "Engine::Engine(): OpenGLApi cannot be null!");
  registerService<const OpenGLApi>(deps.gl);
  assert(deps.platform && "Engine::Engine(): Platform cannot be null!");
  registerService<Platform>(deps.platform);

  registerService<LocalizationSystem>(&localization_);

  const auto* gl = service<const OpenGLApi>();
  shader = std::make_unique<Shader>(std::unordered_map<GLenum, std::string>{{GL_VERTEX_SHADER, "shader.vs.glsl"},
                                                                            {GL_FRAGMENT_SHADER, "shader.fs.glsl"}},
                                    gl);
  uProjection = std::make_unique<UniformMatrix4fv>("uProjection", shader.get());
  uColor = std::make_unique<Uniform4fv>("uColor", shader.get());
  vao = std::make_unique<Vao>(gl);
  vbo = std::make_unique<Vbo>(GL_ARRAY_BUFFER, 0, gl);
  texture = std::make_unique<Texture>(GL_TEXTURE_2D, 0, 0, 0, 0, 0, gl);

  platform_->engine(this);
  platform_->addListener<PlatformEvent::OnCloseRequest>([this](const auto&) { state_.isClosing(true); });
  platform_->addListener<PlatformEvent::OnError>(
      [](const auto& event) { LOG(FATAL) << absl::StrFormat("%i: %s", event.code, event.description); });

  platform_->addListener<PlatformEvent::OnDebugMessage>([](const auto& event) {
    std::string message(event.message, event.length);
    message = absl::StrCat(message, "\n\tID: ", event.id);
    message = absl::StrCat(message, "\n\tSeverity: ", OpenGLApi::GetSeverityString(event.severity));
    message = absl::StrCat(message, "\n\tSource: ", event.source, "\t", OpenGLApi::GetSourceString(event.source));
    message = absl::StrCat(message, "\n\tType: ", event.type, "\t", OpenGLApi::GetTypeString(event.type));
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

  platform_->addListener<PlatformEvent::OnViewportSizeChange>([this](const auto&) {
    auto width = platform_->window()->width();
    auto height = platform_->window()->height();
    const auto* gl = service<const OpenGLApi>();
    gl->viewport(0, 0, static_cast<i32>(width), static_cast<i32>(height));
    dispatchers_.dispatch<EngineEvent::ViewportSizeChange>(ViewportSizeChange(width, height));
    LOG(INFO) << absl::StrFormat("Event: Viewport size change (%u, %u)", width, height);
  });

  platform_->addListener<PlatformEvent::OnMonitorChange>(
      [this](const auto& event) { state_.frameInterval(frameInterval(event.monitor)); });

  constexpr f32 fov = 45;
  constexpr f32 nearPlane = 0.1;
  constexpr f32 farPlane = 5;
  dispatchers_.addListener<EngineEvent::ViewportSizeChange>([](const auto& event) {
    ShaderGuard guard(shader.get());
    *uProjection = glm::perspective(fov, event.aspect(), nearPlane, farPlane);
  });

  std::array<f32, 16> vertices = {
      -0.32f, 0.45f,  0.0f, 1.0f,  // top-left
      0.32f,  0.45f,  1.0f, 1.0f,  // top-right
      -0.32f, -0.45f, 0.0f, 0.0f,  // bottom-left
      0.32f,  -0.45f, 1.0f, 0.0f,  // bottom-right
  };
  {
    VboGuard vbg(vbo.get());
    VaoGuard vag(vao.get());
    vbo->bufferData(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
    vao->linkAttribute(
        {.index = 0, .size = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = 4 * sizeof(GLfloat), .offset = 0});
    vao->linkAttribute({.index = 1,
                        .size = 2,
                        .type = GL_FLOAT,
                        .normalized = GL_FALSE,
                        .stride = 4 * sizeof(GLfloat),
                        .offset = 2 * sizeof(GLfloat)});
  }
  if (auto status = texture->fromFile("texture.jpg"); !status.ok()) {
    LOG(FATAL) << status.message();
  }
}

Engine::~Engine() noexcept {
  vbo.reset();
  vao.reset();
  uColor.reset();
  uProjection.reset();
  shader.reset();
  texture.reset();
}

void Engine::run() noexcept {
  const auto* gl = service<const OpenGLApi>();
  state_.frameInterval(frameInterval(platform_->primaryMonitor().value_or(nullptr)));
  while (!state_.isClosing()) {
    if (auto status = platform_->pollEvents(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    do {
      state_.updateRuntime(runtime());
      advance<EngineStage::PreTick>();
      advance<EngineStage::Tick>();
      advance<EngineStage::PostTick>();
      state_.addTick();
      dispatchers_.dispatch<EngineEvent::TickComplete>(TickComplete(&state_, runtime()));
    } while (state_.runtime() < state_.nextFrame());
    state_.updateRuntime(runtime());
    gl->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    advance<EngineStage::PreRender>();
    advance<EngineStage::Render>();
    advance<EngineStage::PostRender>();
    if (auto status = platform_->swapBuffers(); !status.ok()) {
      LOG(FATAL) << status.message();
    }
    state_.addFrame();
    state_.scheduleNextFrame();
    dispatchers_.dispatch<EngineEvent::RenderComplete>(RenderComplete(&state_, runtime()));
  }
}

void Engine::preTick() noexcept {}

void Engine::tick() noexcept {}

void Engine::postTick() noexcept {}

void Engine::preRender() noexcept {}

void Engine::render() noexcept {
  ShaderGuard shaderGuard(shader.get());
  VaoGuard vaoGuard(vao.get());
  const auto* gl = service<const OpenGLApi>();
  TextureGuard textureGuard(texture.get());
  gl->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Engine::postRender() noexcept {}

}  // namespace uinta
