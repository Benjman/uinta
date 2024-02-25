#ifndef SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_
#define SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_

#include <string>

#include "uinta/camera/camera_comparator.h"
#include "uinta/flags.h"
#include "uinta/scene.h"
#include "uinta/shader.h"
#include "uinta/uniform.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class Camera;

class GridScene : public Scene {
 public:
  struct Flags final {
    using value_type = u8;

    AtomicFlagsOperations(DepthTestMask);

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

   private:
    static constexpr value_type ColorDirtyMask = 1 << 0;
    static constexpr value_type DepthTestMask = 1 << 1;

    std::atomic<value_type> flags_;
  };

  explicit GridScene(Scene*, Engine*, size_t size, const Camera*) noexcept;

  std::string name() const noexcept override { return "GridScene"; }

  void render(const EngineState&) noexcept override;

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
  glm::vec4 color_;
  const Camera* camera_;
  CameraComparator comparator_;
  const OpenGLApi* gl_;
  size_t renderCount_;
  Flags flags_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_SCENES_GRID_H_
