#ifndef SRC_APP_SRC_SCENES_TEXT_H_
#define SRC_APP_SRC_SCENES_TEXT_H_

#include <vector>

#include "uinta/font.h"
#include "uinta/gl.h"
#include "uinta/scene.h"
#include "uinta/shaders/text.h"
#include "uinta/texture.h"
#include "uinta/utils/viewport_change.h"
#include "uinta/vbo.h"

namespace uinta {

class TextScene : public Scene {
 public:
  TextScene() noexcept : Scene(Layer::UI) {
    constexpr f32 width = 512;
    constexpr f32 height = 512;

    std::vector<f32> vtx = {
        // clang-format off
        // position           // color         // uv
        0.0,   0.0,    0.0,   1.0, 1.0, 1.0,   0.0, 0.0,
        0.0,   height, 0.0,   1.0, 1.0, 1.0,   0.0, 1.0,
        width, 0.0,    0.0,   1.0, 1.0, 1.0,   1.0, 0.0,
        width, height, 0.0,   1.0, 1.0, 1.0,   1.0, 1.0,
        // clang-format on
    };

    VboGuard vbg(&vbo_);
    vbo_.bufferData(vtx.data(), vtx.size() * sizeof(f32), GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    shader_.linkAttributes(&vao_);
  }

  void render(const EngineState&) noexcept override {
    ShaderGuard sg(&shader_);
    TextureGuard tg(&font_.texture());
    VaoGuard vg(&vao_);
    ActiveTextureGuard atg;
    BlendGuard bg;
    gl_->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

  void onViewportSizeChange(const ViewportSizeChange& event) noexcept override {
    ShaderGuard sg(&shader_);
    shader_.projection =
        glm::ortho<f32>(0, event.width, event.height, 0, -1, 1);
  }

 private:
  Font font_ = Font("MapleMono-Regular.ttf");
  TextShader shader_;
  Vao vao_;
  Vbo vbo_;
  const OpenGLApi* gl_ = OpenGLApiImpl::GetInstance();
};

}  // namespace uinta

#endif  // SRC_APP_SRC_SCENES_TEXT_H_
