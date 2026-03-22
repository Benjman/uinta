#include "uinta/scenes/grid.h"

#include <imgui.h>

#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "uinta/camera/camera_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/gl.h"
#include "uinta/viewport/viewport_events.h"
#include "uinta/viewport/viewport_manager.h"

namespace uinta {

GridScene::GridScene(Scene* parent, Params params) noexcept
    : Scene(parent, SceneLayer::Simulation),
      shader_(engine()->gl()),
      vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
      params_(params),
      color_(params.color) {
  auto* viewportManager = engine()->service<ViewportManager>();
  assert(viewportManager);
  viewportManager->addListener<ViewportEvent::ProjectionMatrixUpdated>(
      [this](const auto& event) {
        projection_ = event.projection;
        flags_.isProjectionDirty(true);
      });

  auto* cameraManager = engine()->service<CameraManager>();
  assert(cameraManager);
  cameraManager->addListener<CameraEvent::ViewMatrixUpdated>(
      [this](const auto& event) {
        view_ = event.view;
        cameraTarget_ = event.camera->target();
        flags_.isViewDirty(true);
      });

  generate();
}

void GridScene::udpate(Params params) noexcept {
  params_ = params;
  color_ = params.color;
  generate();
}

void GridScene::generate() noexcept {
  std::vector<glm::vec2> vertices;
  vertices.reserve(4 * params_.radius);

  i32 irad = static_cast<i32>(params_.radius);
  f32 frad = static_cast<f32>(params_.radius);

  for (i32 i = -irad; i <= irad; i++) {
    auto if_ = static_cast<f32>(i);
    std::array<glm::vec2, 4> points(
        {{-frad * params_.cellWidth, if_ * params_.cellHeight},
         {frad * params_.cellWidth, if_ * params_.cellHeight},
         {if_ * params_.cellWidth, -frad * params_.cellHeight},
         {if_ * params_.cellWidth, frad * params_.cellHeight}});
    std::ranges::for_each(points.begin(), points.end(),
                          [this](auto& point) { point += params_.position; });
    vertices.insert(vertices.end(), points.begin(), points.end());
  }

  VaoGuard vag(&vao_);
  VboGuard vbg(&vbo_);
  vbo_.bufferData(vertices.data(),
                  static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec2)),
                  GL_STATIC_DRAW);

  shader_.linkAttributes(&vao_);

  renderCount_ = vertices.size();
}

void GridScene::onDebugUi() noexcept {
  auto update =
      ImGui::InputFloat2("Positios##grid", glm::value_ptr(params_.position));
  flags_.isColorDirty(ImGui::ColorEdit4("Color##grid", &color_.r));
  update |= ImGui::InputFloat2("Sizs##grid", &params_.cellWidth) || update;
  auto radius = static_cast<i32>(params_.radius);
  update |= ImGui::InputInt("Radius##grid", &radius) || update;

  if (update) {
    params_.radius = radius;
    flags_.isColorDirty(true);
    generate();
  }
}

void GridScene::render(time_t /*unused*/) noexcept {
  const auto* gl = engine()->gl();
  DepthTestGuard dtg(flags_.isDepthTest(), gl);
  ShaderGuard sg(&shader_);
  BlendGuard bg(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true, gl);

  if (flags_.isColorDirty()) {
    shader_.color = color_;
    flags_.isColorDirty(false);
  }

  if (flags_.isViewDirty()) {
    shader_.modelView = view_;
    shader_.cameraPos = cameraTarget_;

    // Snap grid offset to cell boundaries so grid follows camera
    params_.position = glm::vec2(
        std::floor(cameraTarget_.x / params_.cellWidth) * params_.cellWidth,
        std::floor(cameraTarget_.z / params_.cellHeight) * params_.cellHeight);
    shader_.gridOffset = params_.position;

    flags_.isViewDirty(false);
  }

  if (flags_.isProjectionDirty()) {
    shader_.projection = projection_;
    flags_.isProjectionDirty(false);
  }

  VaoGuard vg(&vao_, gl != nullptr);
  gl->drawArrays(GL_LINES, 0, static_cast<GLsizei>(renderCount_));
}

}  // namespace uinta
