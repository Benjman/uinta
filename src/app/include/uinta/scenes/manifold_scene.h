#ifndef SRC_APP_INCLUDE_UINTA_SCENES_MANIFOLD_SCENE_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_MANIFOLD_SCENE_H_

#include <manifold/manifold.h>

#include "uinta/color.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
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
                    mesh.vertices().size() * Vertex::ElementCount * sizeof(f32),
                    GL_STATIC_DRAW);

    VaoGuard vg(&vao_);
    vao_.ebo(mesh.elements());

    indexCount_ = mesh.elements().size();

    ShaderGuard sg(bsm_->shader());
    bsm_->linkAttributes(&vao_);
  }

  void render(time_t) noexcept override {
    ShaderGuard sg(bsm_->shader());
    CullFaceGuard cfg;
    DepthTestGuard dtg;
    VaoGuard vg(&vao_);

    bsm_->drawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
  }

 private:
  Mesh CreateManifoldDemo() noexcept {
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

    // Add colors to each shape individually
    roundedCube = AddColorToManifold(roundedCube, color::Cyan500);
    cappedCylinder = AddColorToManifold(cappedCylinder, color::Amber800);
    complexShape = AddColorToManifold(complexShape, color::DeepOrange500);

    // Combine all demonstrations (colors will be preserved)
    Manifold result = roundedCube + cappedCylinder + complexShape;

    // Convert manifold to uinta::Mesh
    return ManifoldToMesh(result);
  }

  // Add color properties to a Manifold before combining
  manifold::Manifold AddColorToManifold(const manifold::Manifold& manifold,
                                        const Color& color) noexcept {
    using namespace manifold;

    MeshGL mesh = manifold.GetMeshGL();

    // Input: x, y, z (numProp = 3)
    // Output: x, y, z, r, g, b (numProp = 6)
    size_t oldNumProp = mesh.numProp;
    size_t newNumProp = oldNumProp + 3;  // Add r, g, b
    size_t numVerts = mesh.NumVert();

    std::vector<float> newVertProperties;
    newVertProperties.reserve(numVerts * newNumProp);

    for (size_t i = 0; i < numVerts; ++i) {
      size_t oldOffset = i * oldNumProp;

      // Copy existing properties (x, y, z)
      for (size_t j = 0; j < oldNumProp; ++j) {
        newVertProperties.push_back(
            static_cast<float>(mesh.vertProperties[oldOffset + j]));
      }

      // Add color properties (r, g, b)
      newVertProperties.push_back(color.r);
      newVertProperties.push_back(color.g);
      newVertProperties.push_back(color.b);
    }

    // Create new MeshGL with updated properties
    MeshGL newMesh;
    newMesh.numProp = static_cast<uint32_t>(newNumProp);
    newMesh.vertProperties = newVertProperties;
    newMesh.triVerts = mesh.triVerts;

    return Manifold(newMesh);
  }

  Mesh ManifoldToMesh(const manifold::Manifold& manifold) noexcept {
    // Calculate normals using Manifold's built-in function
    // normalIdx=3 means normals will be properties 6,7,8 (after x,y,z,r,g,b)
    // minSharpAngle=60 means edges sharper than 60 degrees won't be smoothed
    auto withNormals = manifold.CalculateNormals(3, 60.0);
    manifold::MeshGL mMesh = withNormals.GetMeshGL();

    std::vector<Vertex> vertices;
    size_t numVerts = mMesh.NumVert();
    vertices.reserve(numVerts);

    // Check properties:
    // - numProp >= 6: x,y,z,r,g,b
    // - numProp >= 9: x,y,z,r,g,b,nx,ny,nz
    bool hasColors = mMesh.numProp >= 6;
    bool hasNormals = mMesh.numProp >= 9;

    // Convert vertices - properties are interleaved: x,y,z,r,g,b,nx,ny,nz
    for (size_t i = 0; i < numVerts; ++i) {
      size_t offset = i * mMesh.numProp;
      glm::vec3 position{static_cast<float>(mMesh.vertProperties[offset]),
                         static_cast<float>(mMesh.vertProperties[offset + 1]),
                         static_cast<float>(mMesh.vertProperties[offset + 2])};

      Color vertexColor = color::BlueGray100;  // Default color
      if (hasColors) {
        vertexColor =
            Color{static_cast<float>(mMesh.vertProperties[offset + 3]),
                  static_cast<float>(mMesh.vertProperties[offset + 4]),
                  static_cast<float>(mMesh.vertProperties[offset + 5]), 1.0f};
      }

      glm::vec3 normal{0.0f, 0.0f, 1.0f};  // Default normal
      if (hasNormals) {
        normal =
            glm::vec3{static_cast<float>(mMesh.vertProperties[offset + 6]),
                      static_cast<float>(mMesh.vertProperties[offset + 7]),
                      static_cast<float>(mMesh.vertProperties[offset + 8])};
      }

      vertices.emplace_back(Vertex{.position = position,
                                   .normal = normal,
                                   .color = vertexColor,
                                   .uv = Vertex::uv_type{0.0f, 0.0f}});
    }

    // Convert indices
    std::vector<idx_t> elements;
    elements.reserve(mMesh.triVerts.size());
    for (size_t i = 0; i < mMesh.triVerts.size(); ++i) {
      elements.push_back(static_cast<idx_t>(mMesh.triVerts[i]));
    }

    return Mesh(vertices, elements);
  }

  Vao vao_;
  Vbo vbo_;
  size_t indexCount_;
  BasicShaderManager* bsm_;
  const OpenGLApi* gl_;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_MANIFOLD_SCENE_H_
