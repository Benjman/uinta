#include "uinta/scenes/grid.h"

#include <imgui.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "uinta/camera/camera_manager.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/engine_state.h"
#include "uinta/gl.h"

namespace uinta {

GridScene::GridScene(Scene* parent, Params params) noexcept
    : Scene(parent, SCENE_NAME, SceneLayer::Simulation),
      shader_(engine()->gl()),
      vbo_(GL_ARRAY_BUFFER, 0, engine()->gl()),
      params_(params),
      color_(params.color) {
  if (auto copt = parent->findSystem<CameraManager>(); copt.has_value()) {
    copt.value()->addListener<CameraEvent::ProjectionMatrixUpdated>(
        [this](const auto& event) {
          projection_ = event.projection;
          flags_.isProjectionDirty(true);
        });

    copt.value()->addListener<CameraEvent::ViewMatrixUpdated>(
        [this](const auto& event) {
          view_ = event.view;
          flags_.isViewDirty(true);
        });
  } else {
    assert(false && "Failed to find CameraManager*");
  }

  generate();
}

void GridScene::udpate(Params params) noexcept {
  params_ = params;
  color_ = params.color;
  generate();
}

void GridScene::update(Params params) noexcept {
  params_ = params;
  generate();
}

void GridScene::generate() noexcept {
  std::vector<glm::vec2> vertices;
  vertices.reserve(4 * params_.radius);

  i32 irad = static_cast<i32>(params_.radius);
  f32 frad = static_cast<f32>(params_.radius);

  for (i32 i = -irad; i <= irad; i++) {
    std::array<glm::vec2, 4> points(
        {{-frad * params_.cellWidth, i * params_.cellHeight},
         {frad * params_.cellWidth, i * params_.cellHeight},
         {i * params_.cellWidth, -frad * params_.cellHeight},
         {i * params_.cellWidth, frad * params_.cellHeight}});
    std::for_each(points.begin(), points.end(),
                  [this](auto& point) { point += params_.position; });
    vertices.insert(vertices.end(), points.begin(), points.end());
  }

  VaoGuard vag(&vao_);
  VboGuard vbg(&vbo_);
  vbo_.bufferData(vertices.data(), vertices.size() * sizeof(glm::vec2),
                  GL_STATIC_DRAW);

  shader_.linkAttributes(&vao_);

  renderCount_ = vertices.size();
}

void GridScene::onDebugUi() noexcept {
  auto update =
      ImGui::InputFloat3("Position", glm::value_ptr(params_.position));
  update |=
      ImGui::ColorPicker3("Color", glm::value_ptr(params_.color)) || update;
  update |= ImGui::InputFloat2("Size", &params_.cellWidth) || update;
  auto radius = static_cast<i32>(params_.radius);
  update |= ImGui::InputInt("Radius", &radius) || update;
  if (update) {
    params_.radius = radius;
    color_ = params_.color;
    flags_.isColorDirty(true);
    generate();
  }
}

void GridScene::render(time_t) noexcept {
  auto* gl = engine()->gl();
  DepthTestGuard dtg(flags_.isDepthTest(), gl);
  ShaderGuard sg(&shader_);

  if (flags_.isColorDirty()) {
    shader_.color = color_;
    flags_.isColorDirty(false);
  }

  if (flags_.isViewDirty()) {
    shader_.modelView = view_;
    flags_.isViewDirty(false);
  }

  if (flags_.isProjectionDirty()) {
    shader_.projection = projection_;
    flags_.isProjectionDirty(false);
  }

  VaoGuard vg(&vao_, gl);
  gl->drawArrays(GL_LINES, 0, renderCount_);
}

}  // namespace uinta
