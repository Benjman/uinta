#ifndef SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_TYPES_H_
#define SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_TYPES_H_

#include <glm/ext/vector_int2.hpp>
#include <tuple>

#include "uinta/color.h"
#include "uinta/types.h"

namespace uinta {

// Direction bitfield for road connections
enum RoadConnection : u8 {
  None = 0,
  North = 1 << 0,  // +Y in grid space
  South = 1 << 1,  // -Y in grid space
  East = 1 << 2,   // +X in grid space
  West = 1 << 3,   // -X in grid space
};

inline RoadConnection operator|(RoadConnection a, RoadConnection b) noexcept {
  return static_cast<RoadConnection>(static_cast<u8>(a) | static_cast<u8>(b));
}

inline RoadConnection operator&(RoadConnection a, RoadConnection b) noexcept {
  return static_cast<RoadConnection>(static_cast<u8>(a) & static_cast<u8>(b));
}

inline RoadConnection& operator|=(RoadConnection& a,
                                  RoadConnection b) noexcept {
  a = a | b;
  return a;
}

// Road structure
struct Road {
  glm::ivec2 position;
  u8 connections = 0;  // Bitfield of RoadConnection flags
};

// Comparator for std::map (lexicographic ordering)
struct GridPosCompare {
  bool operator()(const glm::ivec2& a, const glm::ivec2& b) const noexcept {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
  }
};

// Utility functions
inline i32 initialRoadCost() { return 5; }

inline f32 roadInflationRate() { return 1.03f; }

inline Color roadConnectionColor(u8 connections) noexcept {
  // Count number of connections
  int count = 0;
  if (connections & RoadConnection::North) count++;
  if (connections & RoadConnection::South) count++;
  if (connections & RoadConnection::East) count++;
  if (connections & RoadConnection::West) count++;

  // Color gradient based on connection count
  switch (count) {
    case 0:
      return color::Gray400;  // Isolated road
    case 1:
      return color::Gray500;  // Dead end
    case 2:
      return color::Gray600;  // Straight path
    case 3:
      return color::Gray700;  // T-junction
    case 4:
      return color::Gray800;  // Crossroads
  }
  return color::Gray400;
}

}  // namespace uinta

#endif  // SRC_APP_INCLUDE_UINTA_SCENES_CITY_GAME_ROAD_TYPES_H_
