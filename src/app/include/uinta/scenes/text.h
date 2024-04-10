#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_

#include <string>

#include "absl/log/log.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/font.h"
#include "uinta/gl.h"
#include "uinta/platform.h"
#include "uinta/scene.h"
#include "uinta/shaders/text.h"
#include "uinta/vbo.h"

namespace uinta {

class TextScene : public Scene {
 public:
  explicit TextScene(Scene* parent, Engine* engine,
                     const DebugSceneParams*) noexcept
      : Scene(parent, engine, Layer::UI),
        font_("font/JetBrains-Mono-Nerd-Font-Complete.ttf"),
        shader_(engine->gl()),
        vbo_(GL_ARRAY_BUFFER, 0, engine->gl()) {
    if (!font_.status().ok()) LOG(FATAL) << font_.status().message();

    auto message = "Translation - edsf\nYaw - wr\nPitch - qa\nDist - cv";

    auto status =
        TextMeshBuilder(&font_, message)
            .align(Alignment::TopLeft)
            .color(glm::vec4(1))
            .dimensions({engine->platform()->window()->width(), 0})
            .lineSize(24)
            .position({50, 50})  // FIXME: Starting y-value of text bleeds too
                                 // far above where it should be.
            .build();

    drawCount_ = strlen(message) * VerticesPerTextQuad;

    auto data = status.value();

    VboGuard vbg(&vbo_);
    vbo_.bufferData(data.data(), data.size() * sizeof(f32), GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    shader_.linkAttributes(&vao_);

    this->engine()->addListener<EngineEvent::ViewportSizeChange>(
        [&](const auto& event) {
          ShaderGuard sg(&shader_);
          shader_.projection =
              glm::ortho<f32>(0, event.width, event.height, 0, -1, 1);
        });
  }

  std::string name() const noexcept override { return "TextScene"; }

  void render(const EngineState&) noexcept override {
    constexpr auto drawStart = 0;
    VaoGuard vg(&vao_);
    shader_.draw(&font_, drawStart, drawCount_);
  }

 private:
  Font font_;
  TextShader shader_;
  Vao vao_;
  Vbo vbo_;
  size_t drawCount_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_
