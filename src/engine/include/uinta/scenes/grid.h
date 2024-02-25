#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_

#include "uinta/color.h"
#include "uinta/flags.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class CameraManager;

class GridScene : public Scene {
 public:
  struct Params final {
    glm::vec2 position;
    std::size_t radius;
    Color color;
    f32 cellWidth;
    f32 cellHeight;

    Params(glm::vec2 position = {0, 0}, std::size_t radius = 30,
           Color color = {78 / 255.0, 68 / 255.0, 62 / 255.0, 1.0},
           f32 cellWidth = 1.0, f32 cellHeight = 1.0) noexcept
        : position(position),
          radius(radius),
          color(color),
          cellWidth(cellWidth),
          cellHeight(cellHeight) {}
  };

  struct Flags final {
    using value_type = u8;

    FlagsOperations(DepthTestMask | ColorDirtyMask);

    bool isDepthTest() const noexcept { return flags_ & DepthTestMask; }
    void isDepthTest(bool v) noexcept {
      flags_ &= ~DepthTestMask;
      if (v) flags_ |= DepthTestMask;
    }

    bool isColorDirty() const noexcept { return flags_ & ColorDirtyMask; }
    void isColorDirty(bool v) noexcept {
      flags_ &= ~ColorDirtyMask;
      if (v) flags_ |= ColorDirtyMask;
    }

    bool isViewDirty() const noexcept { return flags_ & ViewDirtyMask; }
    void isViewDirty(bool v) noexcept {
      flags_ &= ~ViewDirtyMask;
      if (v) flags_ |= ViewDirtyMask;
    }

    bool isProjectionDirty() const noexcept {
      return flags_ & ProjectionDirtyMask;
    }
    void isProjectionDirty(bool v) noexcept {
      flags_ &= ~ProjectionDirtyMask;
      if (v) flags_ |= ProjectionDirtyMask;
    }

   private:
    static constexpr value_type ColorDirtyMask = 1 << 0;
    static constexpr value_type DepthTestMask = 1 << 1;
    static constexpr value_type ViewDirtyMask = 1 << 2;
    static constexpr value_type ProjectionDirtyMask = 1 << 3;

    value_type flags_;
  };

  explicit GridScene(Scene*, Params = Params()) noexcept;

  void render(time_t) noexcept override;

  void onDebugUi() noexcept override;

  void udpate(Params) noexcept;

  void update(Params) noexcept;

  void generate() noexcept;

  Flags flags() const noexcept { return flags_; }

  void flags(Flags flags) noexcept { flags_ = flags; }

 private:
  struct GridShader : public Shader {
    UniformMatrix4fv modelView = {"uModelView", this};
    UniformMatrix4fv projection = {"uProjection", this};
    Uniform4f color = {"uColor", this};

    explicit GridShader(const OpenGLApi* gl) noexcept
        : Shader(
              {
                  {GL_VERTEX_SHADER, "shader/grid.vs.glsl"},
                  {GL_FRAGMENT_SHADER, "shader/grid.fs.glsl"},
              },
              gl) {}

    void linkAttributes(const Vao* vao) const noexcept {
      constexpr Attribute PositionAttrib(0, 2, GL_FLOAT, GL_FALSE,
                                         2 * sizeof(GLfloat), 0);
      vao->linkAttribute(PositionAttrib);
    }
  } shader_;
  Vao vao_;
  Vbo vbo_;
  glm::mat4 view_, projection_;
  Params params_;
  Color color_;
  size_t renderCount_;
  Flags flags_;
};

using GridSceneParams = GridScene::Params;

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_
