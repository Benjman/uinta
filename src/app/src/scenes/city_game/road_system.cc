#include "uinta/scenes/city_game/road_system.h"

#include <algorithm>

namespace uinta {

void RoadSystem::addRoad(glm::ivec2 position) noexcept {
  // Don't add if already exists
  if (hasRoad(position)) return;

  // Insert new road with no connections
  roads_.emplace(position, Road{.position = position, .connections = 0});

  // Clear update tracking
  lastUpdated_.clear();
  lastUpdated_.push_back(position);

  // Update connections for new road and neighbors
  updateConnections(position);
}

bool RoadSystem::hasRoad(glm::ivec2 position) const noexcept {
  return roads_.find(position) != roads_.end();
}

const Road* RoadSystem::getRoad(glm::ivec2 position) const noexcept {
  auto it = roads_.find(position);
  if (it == roads_.end()) return nullptr;
  return &it->second;
}

void RoadSystem::updateConnections(glm::ivec2 position) noexcept {
  auto it = roads_.find(position);
  if (it == roads_.end()) return;

  // Check each cardinal direction
  connectNeighbor(position, position + glm::ivec2(0, 1), RoadConnection::North,
                  RoadConnection::South);
  connectNeighbor(position, position + glm::ivec2(0, -1),
                  RoadConnection::South, RoadConnection::North);
  connectNeighbor(position, position + glm::ivec2(1, 0), RoadConnection::East,
                  RoadConnection::West);
  connectNeighbor(position, position + glm::ivec2(-1, 0), RoadConnection::West,
                  RoadConnection::East);
}

void RoadSystem::connectNeighbor(glm::ivec2 pos, glm::ivec2 neighborPos,
                                  RoadConnection outgoing,
                                  RoadConnection incoming) noexcept {
  auto neighborIt = roads_.find(neighborPos);
  if (neighborIt == roads_.end()) return;

  // Set bidirectional connections
  roads_[pos].connections |= static_cast<u8>(outgoing);
  neighborIt->second.connections |= static_cast<u8>(incoming);

  // Track neighbor for re-render
  if (std::find(lastUpdated_.begin(), lastUpdated_.end(), neighborPos) ==
      lastUpdated_.end()) {
    lastUpdated_.push_back(neighborPos);
  }
}

}  // namespace uinta
