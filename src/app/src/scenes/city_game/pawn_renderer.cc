#include "uinta/scenes/city_game/pawn_renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "uinta/mesh.h"

namespace uinta {

Color PawnRenderer::getPawnColor(PawnState state) noexcept {
  switch (state) {
    case PawnState::Idle:
      return color::White;
    case PawnState::Walking:
      return color::Gray400;
    case PawnState::Working:
      return color::Blue500;
    case PawnState::Eating:
      return color::Orange500;
    case PawnState::Sleeping:
      return color::DeepPurple700;
  }
  return color::White;
}

PawnRenderer::PawnRenderer(BasicShaderManager* bsm,
                           const OpenGLApi* gl) noexcept
    : bsm_(bsm), gl_(gl) {
  initStateMeshes();
}

void PawnRenderer::initStateMeshes() noexcept {
  // Create a cube mesh for each pawn state with appropriate color
  for (size_t s = 0; s < STATE_COUNT; ++s) {
    PawnState state = static_cast<PawnState>(s);
    Color col = getPawnColor(state);

    idx_t idxOffset = 0;
    auto mesh = Mesh::Cube(&idxOffset);
    mesh.color(glm::vec3(col));

    // Create VAO and VBO for this state
    vaos_[s] = std::make_unique<Vao>(gl_);
    vbos_[s] = std::make_unique<Vbo>(GL_ARRAY_BUFFER, 0, gl_);

    VaoGuard vag(vaos_[s].get());
    VboGuard vbg(vbos_[s].get());

    auto* data = mesh.vertices().data();
    auto bufSize = mesh.vertices().size() * Vertex::ElementCount * sizeof(f32);
    vbos_[s]->bufferData(data, bufSize, GL_STATIC_DRAW);

    vaos_[s]->ebo(mesh.elements());
    bsm_->linkAttributes(vaos_[s].get());

    if (s == 0) {
      cubeIndexCount_ = mesh.elements().size();
    }
  }
}

void PawnRenderer::render(const Pawns& pawns) noexcept {
  if (pawns.empty()) return;

  DepthTestGuard dtg;
  CullFaceGuard cfg;
  ShaderGuard sg(bsm_->shader());

  // Pawn dimensions: 0.5 x 1.0 x 0.5
  constexpr glm::vec3 pawnScale(0.5f, 1.0f, 0.5f);

  for (size_t i = 0; i < pawns.count(); ++i) {
    // Skip pawns inside buildings (not visible)
    if (pawns.currentBuilding[i] != NULL_BUILDING) {
      continue;
    }

    glm::vec3 pos = pawns.positions[i];
    PawnState state = pawns.states[i];
    size_t stateIdx = static_cast<size_t>(state);

    // Model matrix: translate to position, pivot at feet, scale
    glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::translate(model, glm::vec3(0, 0.5f, 0));  // pivot at feet
    model = glm::scale(model, pawnScale);

    bsm_->model(model);

    // Use the VAO for this state's color
    VaoGuard vg(vaos_[stateIdx].get());
    bsm_->drawElements(GL_TRIANGLES, static_cast<GLsizei>(cubeIndexCount_),
                       GL_UNSIGNED_INT, nullptr);
  }

  bsm_->model(glm::mat4(1));
}

}  // namespace uinta
