#ifndef UINTA_GENERATORS_H
#define UINTA_GENERATORS_H

struct quad;

extern void generateMesh(const quad *qt, float *vertexBuffer, unsigned int *indexBuffer, unsigned int *vertexCount, unsigned int *indexCount, unsigned int *indexOffset, const unsigned int width, const unsigned int height) noexcept;

#endif // UINTA_GENERATORS_H
