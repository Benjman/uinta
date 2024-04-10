#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_

#include <cstring>
#include <string>
#include <vector>

#include "absl/log/log.h"
#include "glm/ext/vector_float4.hpp"
#include "uinta/debug/debug_scene.h"
#include "uinta/engine.h"
#include "uinta/font.h"
#include "uinta/gl.h"
#include "uinta/platform.h"
#include "uinta/scene.h"
#include "uinta/shaders/text.h"
#include "uinta/utils/viewport_change.h"
#include "uinta/vbo.h"

namespace uinta {

class TextScene : public Scene {
 public:
  explicit TextScene(Scene* parent, const DebugSceneParams& params) noexcept
      : Scene(parent, Layer::UI) {
    if (!font_.status().ok()) LOG(FATAL) << font_.status().message();

    auto message = "Translation - edsf\nYaw - wr\nPitch - qa\nDist - cv";

    auto status =
        TextMeshBuilder(&font_, message)
            .align(Alignment::TopLeft)
            .color(glm::vec4(1))
            .dimensions({params.engine->platform()->window()->width(), 0})
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
  }

  void render(const EngineState&) noexcept override {
    constexpr auto drawStart = 0;
    VaoGuard vg(&vao_);
    shader_.draw(&font_, drawStart, drawCount_);
  }

  void onViewportSizeChange(const ViewportSizeChange& event) noexcept override {
    ShaderGuard sg(&shader_);
    shader_.projection =
        glm::ortho<f32>(0, event.width, event.height, 0, -1, 1);
  }

 private:
  Font font_ = Font("font/JetBrains-Mono-Nerd-Font-Complete.ttf");
  TextShader shader_;
  Vao vao_;
  Vbo vbo_;
  size_t drawCount_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_
