#ifndef UINTA_MESH_HPP
#define UINTA_MESH_HPP

#include <uinta/types.h>

#include <string>
#include <unordered_map>

namespace uinta {

enum class MeshAttribute : u8 {
  Color,
  Normal,
  Position,
  UV,
  None,
};

struct MeshLocation {
  u32 stride;
  u32 offset;
};

inline bool location(MeshLocation& ref, MeshAttribute attribute,
                     const std::unordered_map<MeshAttribute, MeshLocation>& collection) {
  if (collection.find(attribute) == collection.end()) return false;
  ref = collection.at(attribute);
  return true;
}

using MeshLayout = std::unordered_map<MeshAttribute, MeshLocation>;

// TODO: Below needs to be entirely refactored.

enum class MeshAttribType : u8 {
  Color,
  Normal,
  Position,
  UV,
};

struct MeshAttrib final {
  u32 stride;
  u32 offset;

  MeshAttrib(const u32 stride, const u32 offset) noexcept : stride(stride), offset(offset) {
  }
};

MeshAttrib findMeshAttrib(MeshAttribType, const std::unordered_map<MeshAttribType, MeshAttrib>&);

bool hasMeshAttrib(MeshAttribType, const std::unordered_map<MeshAttribType, MeshAttrib>&);

inline std::string to_string(MeshAttribType type) {
  switch (type) {
    case MeshAttribType::Color:
      return "Color";
    case MeshAttribType::Normal:
      return "Normal";
    case MeshAttribType::Position:
      return "Position";
    case MeshAttribType::UV:
      return "UV";
    default:
      return "Unknown";
  }
}

}  // namespace uinta

#endif  // UINTA_MESH_HPP
