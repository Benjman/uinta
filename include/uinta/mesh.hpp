#ifndef UINTA_MESH_HPP
#define UINTA_MESH_HPP

#include <uinta/types.h>

#include <string>
#include <unordered_map>

namespace uinta {

enum class MeshAttribType {
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
      return "MeshAttribType_Color";
    case MeshAttribType::Normal:
      return "MeshAttribType_Normal";
    case MeshAttribType::Position:
      return "MeshAttribType_Position";
    case MeshAttribType::UV:
      return "MeshAttribType_UV";
    default:
      return "UNKNOWN MeshAttribType";
  }
}

}  // namespace uinta

#endif  // UINTA_MESH_HPP
