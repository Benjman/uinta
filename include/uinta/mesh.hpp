#ifndef UINTA_GENERATORS_H
#define UINTA_GENERATORS_H

#include <unordered_map>

struct Quad;

extern void generateMesh(const Quad *qt, float *vertexBuffer, unsigned int *indexBuffer, unsigned int *vertexCount,
                         unsigned int *indexCount, unsigned int *indexOffset, const unsigned int width,
                         const unsigned int height) noexcept;

enum MeshAttribType {
  MeshAttribType_Color,
  MeshAttribType_Normal,
  MeshAttribType_Position,
  MeshAttribType_UV,
};

struct MeshAttrib final {
  unsigned int stride;
  unsigned int offset;

  MeshAttrib(const unsigned int stride, const unsigned int offset) noexcept : stride(stride), offset(offset) {}

  MeshAttrib(const MeshAttrib &other) noexcept { *this = other; }

  MeshAttrib &operator=(const MeshAttrib &other) noexcept {
    stride = other.stride;
    offset = other.offset;
    return *this;
  }
};

const MeshAttrib *findMeshAttrib(
    MeshAttribType,
    const std::unordered_map<MeshAttribType, MeshAttrib> *); // TODO this should take a reference to an unordered_map, not a ptr

#endif // UINTA_GENERATORS_H
