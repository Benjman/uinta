#ifndef SRC_APP_INCLUDE_UINTA_SCENES_HEXAGON_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_HEXAGON_H_

#include "uinta/camera/camera.h"
#include "uinta/debug/debug_scene.h"
#include "uinta/gl.h"
#include "uinta/math/direction.h"
#include "uinta/mesh.h"
#include "uinta/scene.h"
#include "uinta/shaders/primitive.h"
#include "uinta/utils/hexagon.h"
#include "uinta/vao.h"

namespace uinta {

class HexagonScene : public Scene {
 public:
  explicit HexagonScene(Scene* parent, const DebugSceneParams& params) noexcept
      : Scene(parent, Layer::Simulation), shader_(params.shader) {
    HexagonGrid grid(1, 4);
    auto hexagons = grid.generate();
    hexagonCount_ = hexagons.size();

    Mesh hexagonPrimitive;
    for (auto& hexagon : hexagons) {
      constexpr auto toVertex = [](glm::vec2 point) {
        return Vertex(glm::vec3(point.x, 0, point.y), WorldUp, glm::vec3(1));
      };
      const auto& points = hexagon.vertexPoints(grid.hexagonSize());
      for (auto& point : points) {
        hexagonPrimitive.addVertex(toVertex(point));
      }
      hexagonPrimitive.addVertex(toVertex(points.at(0)));
    }

    VboGuard vbg(&vbo_);
    vbo_.bufferData(hexagonPrimitive.vertices().data(),
                    hexagonPrimitive.vertices().size() * Vertex::ElementCount *
                        sizeof(glm::vec2),
                    GL_STATIC_DRAW);

    VaoGuard vag(&vao_);
    shader_->linkAttributes(&vao_);
  }

  void render(const EngineState&) noexcept override {
    DepthTestGuard dtg;
    CullFaceGuard cfg;
    ShaderGuard sg(shader_);
    VaoGuard vg(&vao_);
    shader_->model = glm::mat4(1);
    shader_->sway(false);

    for (size_t i = 0; i < hexagonCount_; i++) {
      gl_->drawArrays(GL_LINE_STRIP, 7 * i, 7);
    }
  }

 private:
  Vao vao_;
  Vbo vbo_;
  const OpenGLApi* gl_ = OpenGLApiImpl::Instance();
  PrimitiveShader* shader_;
  size_t hexagonCount_ = 0;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_HEXAGON_H_
