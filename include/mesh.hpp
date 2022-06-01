#ifndef UINTA_GENERATORS_H
#define UINTA_GENERATORS_H

struct quad;

extern void generateMesh(const quad *qt, float *vertexBuffer, unsigned int *indexBuffer, unsigned int *vertexCount, unsigned int *indexCount, unsigned int *indexOffset, const unsigned int width, const unsigned int height) noexcept;

enum MeshAttribType {
    MeshAttribType_Color,
    MeshAttribType_Normal,
    MeshAttribType_Position,
    MeshAttribType_UV,
};

struct mesh_attrib final {
    unsigned int stride;
    unsigned int offset;

    mesh_attrib(const unsigned int stride, const unsigned int offset) noexcept :
        stride(stride), offset(offset) {}

    mesh_attrib(const mesh_attrib& other) noexcept {
        *this = other;
    }

    mesh_attrib& operator=(const mesh_attrib& other) noexcept {
        stride = other.stride;
        offset = other.offset;
        return *this;
    }
};

#include <unordered_map>
const mesh_attrib* find_mesh_attrib(MeshAttribType, const std::unordered_map<MeshAttribType, mesh_attrib>*); // TODO this should take a reference to an unordered_map, not a ptr

#endif // UINTA_GENERATORS_H
