#ifndef UINTA_MESH_HPP
#define UINTA_MESH_HPP

#include <uinta/types.h>

#include <unordered_map>

namespace uinta {

struct Quad;

extern void generateMesh(const Quad* qt, f32* vertexBuffer, u32* indexBuffer, u32* vertexCount, u32* indexCount, u32* indexOffset,
                         const u32 width, const u32 height) noexcept;

enum MeshAttribType {
  MeshAttribType_Color = 0,
  MeshAttribType_Normal = 1,
  MeshAttribType_Position = 2,
  MeshAttribType_UV = 3,
};

struct MeshAttrib final {
  u32 stride;
  u32 offset;

  MeshAttrib(const u32 stride, const u32 offset) noexcept : stride(stride), offset(offset) {
  }

  MeshAttrib(const MeshAttrib& other) noexcept {
    *this = other;
  }

  MeshAttrib& operator=(const MeshAttrib& other) noexcept {
    stride = other.stride;
    offset = other.offset;
    return *this;
  }
};

MeshAttrib findMeshAttrib(MeshAttribType, const std::unordered_map<MeshAttribType, MeshAttrib>&);
bool hasMeshAttrib(MeshAttribType, const std::unordered_map<MeshAttribType, MeshAttrib>&);

}  // namespace uinta

#endif  // UINTA_MESH_HPP
