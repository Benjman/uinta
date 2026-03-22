#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_SYSTEM_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_SYSTEM_H_

#include <map>
#include <vector>

#include "./road_types.h"

namespace uinta {

class RoadSystem {
 public:
  RoadSystem() = default;

  // Core operations
  void addRoad(glm::ivec2 position) noexcept;
  bool hasRoad(glm::ivec2 position) const noexcept;
  const Road* getRoad(glm::ivec2 position) const noexcept;

  // Queries
  size_t count() const noexcept { return roads_.size(); }
  bool empty() const noexcept { return roads_.empty(); }

  // Access
  const std::map<glm::ivec2, Road, GridPosCompare>& roads() const noexcept {
    return roads_;
  }

  // Returns list of positions whose connections were updated
  const std::vector<glm::ivec2>& getLastUpdatedRoads() const noexcept {
    return lastUpdated_;
  }

 private:
  std::map<glm::ivec2, Road, GridPosCompare> roads_;
  std::vector<glm::ivec2> lastUpdated_;  // Track changes for re-render

  void updateConnections(glm::ivec2 position) noexcept;
  void connectNeighbor(glm::ivec2 pos, glm::ivec2 neighborPos,
                       RoadConnection outgoing,
                       RoadConnection incoming) noexcept;
};

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_SYSTEM_H_
