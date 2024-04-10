#ifndef SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_

#include "absl/log/log.h"
#include "uinta/debug/debug_scene_ui.h"
#include "uinta/engine/engine.h"
#include "uinta/flags.h"
#include "uinta/font.h"
#include "uinta/gl.h"
#include "uinta/platform.h"
#include "uinta/scene/scene.h"
#include "uinta/shaders/text_shader.h"
#include "uinta/vbo.h"

namespace uinta {

class TextScene : public Scene {
 public:
  explicit TextScene(Scene* parent) noexcept
      : Scene(parent, SCENE_NAME, SceneLayer::UI),
        font_("font/JetBrains-Mono-Nerd-Font-Complete.ttf"),
        shader_(engine()->gl()),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()) {
    if (!font_.status().ok()) LOG(FATAL) << font_.status().message();

    auto message = "Translation - edsf\nYaw - wr\nPitch - qa\nDist - cv";

    auto status =
        TextMeshBuilder(&font_, message)
            .align(Alignment::TopLeft)
            .color(glm::vec4(1))
            .dimensions({engine()->platform()->window()->width(), 0})
            .lineSize(24)
            .position({50, 50})  // FIXME: Starting y-value of text bleeds too
                                 // far above where it is expected.
            .build();

    drawCount_ = strlen(message) * VerticesPerTextQuad;

    auto data = status.value();

    VboGuard vbg(&vbo_);
    vbo_.bufferData(data.data(), data.size() * sizeof(f32), GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    shader_.linkAttributes(&vao_);

    engine()->dispatchers()->addListener<EngineEvent::ViewportSizeChange>(
        [this](const auto& event) {
          flags_.isProjectionDirty(true);
          projection_ = glm::ortho<f32>(0, event.width, event.height, 0, -1, 1);
        });
  }

  void render(time_t) noexcept override {
    constexpr auto drawStart = 0;
    VaoGuard vg(&vao_);
    ShaderGuard guard(&shader_);
    TextureGuard tg(&font_.texture());
    ActiveTextureGuard atg(GL_TEXTURE0);
    BlendGuard bg(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true);
    if (flags_.isProjectionDirty()) {
      shader_.projection = projection_;
      flags_.isProjectionDirty(false);
    }
    engine()->gl()->drawArrays(GL_TRIANGLES, drawStart, drawCount_);
  }

 private:
  struct Flags final {
    using value_type = u8;

    FlagsOperations(ProjectionDirtyMask);

    bool isProjectionDirty() const noexcept {
      return flags_ & ProjectionDirtyMask;
    }
    void isProjectionDirty(bool v) noexcept {
      flags_ &= ~ProjectionDirtyMask;
      if (v) flags_ |= ProjectionDirtyMask;
    }

    bool isViewDirty() const noexcept { return flags_ & ViewDirtyMask; }
    void isViewDirty(bool v) noexcept {
      flags_ &= ~ViewDirtyMask;
      if (v) flags_ |= ViewDirtyMask;
    }

   private:
    static constexpr value_type ProjectionDirtyMask = 1 << 0;
    static constexpr value_type ViewDirtyMask = 1 << 1;

    value_type flags_;
  } flags_;
  Font font_;
  TextShader shader_;
  Vao vao_;
  Vbo vbo_;
  glm::mat4 projection_;
  size_t drawCount_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_TEXT_H_
