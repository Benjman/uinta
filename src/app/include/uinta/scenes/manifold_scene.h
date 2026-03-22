#ifndef SRC_APP_INCLUDE_UINTA_SCENES_MANIFOLD_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_MANIFOLD_SCENE_H_

#include <manifold/manifold.h>

#include "uinta/color.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/manifold_mesh.h"
#include "uinta/mesh.h"
#include "uinta/scene/scene.h"
#include "uinta/shader.h"
#include "uinta/shaders/basic_shader.h"
#include "uinta/vao.h"
#include "uinta/vbo.h"

namespace uinta {

class ManifoldScene : public Scene {
 public:
  explicit ManifoldScene(Scene* parent) noexcept
      : Scene(parent, SceneLayer::Simulation),
        vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
        bsm_(findComponent<BasicShaderManager>().value_or(nullptr)),
        gl_(engine()->gl()) {
    auto mesh = CreateManifoldDemo();

    VboGuard vbg(&vbo_);
    vbo_.bufferData(mesh.vertices().data(),
                    static_cast<GLsizeiptr>(mesh.vertices().size() *
                                            Vertex::ElementCount * sizeof(f32)),
                    GL_STATIC_DRAW);

    VaoGuard vg(&vao_);
    vao_.ebo(mesh.elements());

    indexCount_ = mesh.elements().size();

    ShaderGuard sg(bsm_->shader());
    bsm_->linkAttributes(&vao_);
  }

  void render(time_t /*unused*/) noexcept override {
    ShaderGuard sg(bsm_->shader());
    CullFaceGuard cfg;
    DepthTestGuard dtg;
    VaoGuard vg(&vao_);

    bsm_->drawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_),
                       GL_UNSIGNED_INT, nullptr);
  }

 private:
  static Mesh CreateManifoldDemo() noexcept {
    using namespace manifold;

    // Create some basic primitives
    Manifold sphere = Manifold::Sphere(1.0, 48);
    Manifold cube = Manifold::Cube({2.0, 2.0, 2.0}, true);
    Manifold cylinder = Manifold::Cylinder(0.5, 3.0, -1.0, 64);

    // Demonstrate CSG operations
    // 1. Create a rounded cube by intersecting a cube with a sphere
    Manifold roundedCube =
        cube.Translate({-4.5, 0.0, 0.0}) ^
        sphere.Scale({1.5, 1.5, 1.5}).Translate({-4.5, 0.0, 0.0});

    // 3. Create a cylinder with spherical caps removed
    Manifold cappedCylinder =
        cylinder.Rotate(0.0, 90.0, 90.0) -
        (sphere.Scale({0.6, 0.6, 0.6}).Translate({1.2, 0.25, 0.0}) +
         sphere.Scale({0.6, 0.6, 0.6}).Translate({-1.2, 0.25, 0.0}));

    // 4. Create a more complex shape: sphere with cylindrical holes
    Manifold holedSphere =
        sphere.Scale({1.2, 1.2, 1.2}).Translate({4.5, 0.0, 0.0});
    Manifold xHole = Manifold::Cylinder(0.4, 3.0, -1.0, 32)
                         .Rotate(0.0, 90.0, 0.0)
                         .Translate({4.0, 0.0, 0.0});
    Manifold yHole =
        Manifold::Cylinder(0.4, 3.0, -1.0, 32).Translate({4.0, 0.0, 0.0});
    Manifold zHole = Manifold::Cylinder(0.4, 3.0, -1.0, 32)
                         .Rotate(90.0, 0.0, 0.0)
                         .Translate({4.0, 0.0, 0.0});

    Manifold complexShape = holedSphere - xHole - yHole - zHole;

    // Convert each shape to a uinta::Mesh independently, tint it, then
    // combine with rebased indices. This sidesteps carrying color as a
    // Manifold property channel (which previously collided with
    // CalculateNormals' normal channels).
    auto colorize = [](Mesh mesh, const Color& c) {
      mesh.color(glm::vec3(c.r, c.g, c.b));
      return mesh;
    };

    // Fully qualify ToMesh because `using namespace manifold;` above brings
    // manifold:: into scope; without the qualifier unqualified lookup can be
    // ambiguous in some compiler configurations.
    std::array<Mesh, 3> subMeshes = {
        colorize(ToMesh(roundedCube), color::Cyan500),
        colorize(ToMesh(cappedCylinder), color::Amber800),
        colorize(ToMesh(complexShape), color::DeepOrange500),
    };

    Mesh result;
    size_t totalVerts = 0;
    size_t totalElems = 0;
    for (const auto& m : subMeshes) {
      totalVerts += m.vertices().size();
      totalElems += m.elements().size();
    }
    result.reserve(totalVerts, totalElems);

    idx_t offset = 0;
    for (const auto& m : subMeshes) {
      for (const auto& v : m.vertices()) {
        result.addVertex(v);
      }
      // Rebase and append indices by round-tripping through a temporary
      // mesh with no vertices, since Mesh has no public addElement.
      std::vector<idx_t> rebased;
      rebased.reserve(m.elements().size());
      for (const auto idx : m.elements()) {
        rebased.push_back(idx + offset);
      }
      Mesh indicesOnly(std::span<const Vertex>{}, rebased);
      result.merge(indicesOnly);
      offset += static_cast<idx_t>(m.vertices().size());
    }

    return result;
  }

  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
  BasicShaderManager* bsm_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_MANIFOLD_SCENE_H_
