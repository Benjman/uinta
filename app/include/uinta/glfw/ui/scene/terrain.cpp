#include <imgui/imgui.h>

#include <uinta/math/hexagon.hpp>
#include <uinta/math/utils.hpp>
#include <uinta/runner/runner.hpp>
#include <uinta/utils/direction.hpp>

namespace uinta {

inline void cellInfo(Runner& runner) {
}

// inline void terrain(Runner& runner) {
//   if (!ImGui::TreeNode("Terrain")) return;
//
//   const auto& camera = runner.scene.camera;
//   const auto view = getViewMatrix(camera);
//   const auto proj = getPerspectiveMatrix(camera);
//   const glm::vec2 cursor = {runner.input.cursorx, runner.input.cursory};
//   const glm::vec2 viewport = {runner.window.width, runner.window.height};
//   const auto worldRay = getWorldRay(cursor, viewport, view, proj);
//   const auto worldPoint = getPlaneInterceptPoint(glm::vec3(0), WORLD_UP, camera.position, worldRay);
//
//   const auto axialCoord = world_to_axial({worldPoint.x, worldPoint.z}, {0, 0}, TerrainManager::HexagonRadius);
//   const auto cubeCoord = axial_to_cube(axialCoord);
//
//   ImGui::Text("Axial coord: (%+i %+i)", axialCoord.x, axialCoord.y);
//   ImGui::Text("Cube coord: (%+i %+i %+i)", cubeCoord.x, cubeCoord.y, cubeCoord.z);
//
//   ImGui::TreePop();
//   ImGui::Separator();
// }

}  // namespace uinta
