#ifndef UINTA_MESH_HPP
#define UINTA_MESH_HPP

#include <unordered_map>

namespace uinta {

struct Quad;

extern void generateMesh(const Quad* qt, float* vertexBuffer, unsigned int* indexBuffer, unsigned int* vertexCount,
                         unsigned int* indexCount, unsigned int* indexOffset, const unsigned int width,
                         const unsigned int height) noexcept;

enum MeshAttribType {
  MeshAttribType_Color = 0,
  MeshAttribType_Normal = 1,
  MeshAttribType_Position = 2,
  MeshAttribType_UV = 3,
};

struct MeshAttrib final {
  unsigned int stride;
  unsigned int offset;

  MeshAttrib(const unsigned int stride, const unsigned int offset) noexcept : stride(stride), offset(offset) {
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
